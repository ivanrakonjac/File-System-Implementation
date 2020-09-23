#include "KernelFile.h"

KernelFile::KernelFile(Partition* p, string fName, int firstLvlIndex, int fSize, char m)
{
	numReaders = 0;
	numWriters = 0;
	numWaitRead = 0;
	numWaitWrite = 0;

	partition = p;
	name = fName;
	index1ClusterNumber = firstLvlIndex;
	size = fSize;
	mode = m;
	cursor = 0;

	partition->writeCluster(index1ClusterNumber, index1Cluster);

	index2ClusterNumber=0;
	dataClusterNumber=0;

	index1FreeEntriesNum = ClusterSize/4;
	index2FreeEntriesNum = ClusterSize / 4;
	dataFreeSpaceNum = ClusterSize;
}

char KernelFile::write(unsigned long bytesCnt, char* buffer)
{
	
	int index2 = getIndex2Cluster();
	int data = getDataCluster();

	BytesCnt numOfWrittenBytes = 0;

	if (index2 > -1 && data > -1) {
		while (numOfWrittenBytes != bytesCnt) {

			if (dataFreeSpaceNum == 0) {
				partition->writeCluster(data, tmpDataCluster);
				data = getDataCluster();
				if (data == -1) return numOfWrittenBytes;
			}

			tmpDataCluster[numOfWrittenBytes%ClusterSize] = buffer[numOfWrittenBytes];
			numOfWrittenBytes++;
			size++;
			dataFreeSpaceNum--;

		}

		seek (numOfWrittenBytes);

		partition->writeCluster(dataClusterNumber, tmpDataCluster);
		partition->writeCluster(index2ClusterNumber, tmpIndex2Cluster);
		partition->writeCluster(index1ClusterNumber, index1Cluster);
		return '1';
	}
	else {
		KernelFS::deallocateCluster(index2);
		KernelFS::deallocateCluster(data);
		return '0';
	}
}


char KernelFile::printIndexesStruct()
{
	IndexCluster* ind1Cluster = (IndexCluster*)index1Cluster;

	for (int i = 0; i < 512; i++) {
		char niz[2048];
		cout << "index1 [" << i << "]:="<< ind1Cluster[i].entry << endl;
		if (ind1Cluster[i].entry == 0) break;
		partition->readCluster(ind1Cluster[i].entry, niz);
		IndexCluster* index2 = (IndexCluster*)niz;
		for (int j = 0; j < 512; j++) {
			cout << index2[j].entry << " ";
		}
		cout << endl;
	}

	return '1';
}

char KernelFile::deleteFile()
{
	seek(0);
	truncate();
	return 1;
}

void KernelFile::addCursorForThread()
{
	threadCursorMap.insert({ std::this_thread::get_id(), cursor });
}

int KernelFile::getIndex2Cluster() {
	int index2 = KernelFS::allocateCluster();
	int numOfEntries = ClusterSize / 4;

	IndexCluster* ind1Cluster =(IndexCluster*) index1Cluster;

	if (index2 > -1) {
		index2ClusterNumber = index2;
		ind1Cluster[numOfEntries - index1FreeEntriesNum].entry = index2;
		index1FreeEntriesNum--;

		for (int i = 0; i < ClusterSize; i++)
		{
			tmpIndex2Cluster[i] = 0;
		}
		index2FreeEntriesNum = numOfEntries;
	}

	return index2;
}

int KernelFile::getDataCluster() {
	int data = KernelFS::allocateCluster();
	int numOfEntries = ClusterSize / 4;

	IndexCluster* tmpInd2Cluster = (IndexCluster*)tmpIndex2Cluster;

	if (data > -1) {
		dataClusterNumber = data;
		tmpInd2Cluster[numOfEntries - index2FreeEntriesNum].entry = data;
		index2FreeEntriesNum--;

		if (index2FreeEntriesNum == 0) {
			partition->writeCluster(index2ClusterNumber, tmpIndex2Cluster);
			int index2 = getIndex2Cluster();
			if (index2 == -1) return -1;
		}

		for (int i = 0; i < ClusterSize; i++)
		{
			tmpDataCluster[i] = 0;
		}
		dataFreeSpaceNum = ClusterSize;
	}

	return data;
}

unsigned long KernelFile::read(unsigned long cnt, char* buffer)
{
	//svaka nit koja cita ima svoj kursor
	auto it = threadCursorMap.find(std::this_thread::get_id());
	cursor = it->second;

	int dataClusterPoRedu = cursor / ClusterSize;
	int positionInDataCluster = cursor % ClusterSize;
	int index2ClusterPoRedu = dataClusterPoRedu / 512;
	
	int brojPodataka = 0;
	if (size - cursor > cnt) brojPodataka = cnt;
	else brojPodataka = size - cursor;

	partition->readCluster(getIndexOfIndex2Cluster(index2ClusterPoRedu),tmpIndex2Cluster); //dohvati mi index2Claster koji mi treba
	partition->readCluster(getIndexOfDataCluster(dataClusterPoRedu), tmpDataCluster); //dohvati mi dataClaster koji mi treba

	int brojac = 0;

	while (brojac < brojPodataka) {
		if (positionInDataCluster % ClusterSize == 0 && positionInDataCluster!=0) { //ako sam iscitao ceo dataClaster
			if (dataClusterPoRedu == 512 - 1) { //ako je slucajno taj data cluster 512 po redu (poslednji) => novi index2Cluster
				index2ClusterPoRedu++;
				partition->readCluster(getIndexOfIndex2Cluster(index2ClusterPoRedu), tmpIndex2Cluster);
				dataClusterPoRedu=0;
				partition->readCluster(getIndexOfDataCluster(dataClusterPoRedu), tmpDataCluster);
			}
			else {
				dataClusterPoRedu++;
				partition->readCluster(getIndexOfDataCluster(dataClusterPoRedu), tmpDataCluster);
			}
		}
		
		buffer[brojac] = tmpDataCluster[positionInDataCluster % ClusterSize];
		brojac++;
		positionInDataCluster++;
	}

	seek(cursor + brojac);

	return brojac;
}

char KernelFile::append(unsigned long bytesCnt, char* buffer)
{
	if (size == 0) {
		return write(bytesCnt, buffer);
	}

	seek(size);

	auto it = threadCursorMap.find(std::this_thread::get_id());
	cursor = it->second;

	int dataClusterPoRedu = cursor / ClusterSize;
	int positionInDataCluster = cursor % ClusterSize;
	int index2ClusterPoRedu = dataClusterPoRedu / (ClusterSize / 4);

	index1FreeEntriesNum = (ClusterSize / 4) - index2ClusterPoRedu;
	index2FreeEntriesNum = (ClusterSize / 4) - dataClusterPoRedu;
	dataFreeSpaceNum = ClusterSize - (size % 2048);

	int data;
	int index2;

	if (index2FreeEntriesNum == 0) {
		index2 = getIndex2Cluster();
	}

	if (ClusterSize - (size % 2048) == 2048) {
		data = getDataCluster();
		if (data == '-1') return '0';
	}
	else {
		index2 = getIndexOfIndex2Cluster(index2ClusterPoRedu);
		data = getIndexOfDataCluster(dataClusterPoRedu);
		partition->readCluster(data, tmpDataCluster);
		partition->readCluster(index2, tmpIndex2Cluster);
	}

	int numOfWrittenBytes = 0;

	while (numOfWrittenBytes != bytesCnt) {

		if (dataFreeSpaceNum == 0) {
			partition->writeCluster(data, tmpDataCluster);
			data = getDataCluster();
			if (data == -1) return numOfWrittenBytes;
		}

		tmpDataCluster[positionInDataCluster % ClusterSize] = buffer[numOfWrittenBytes];
		numOfWrittenBytes++;
		positionInDataCluster++;
		size++;
		dataFreeSpaceNum--;

	}

	seek(size);

	partition->writeCluster(dataClusterNumber, tmpDataCluster);
	partition->writeCluster(index2ClusterNumber, tmpIndex2Cluster);
	partition->writeCluster(index1ClusterNumber, index1Cluster);

	return '1';
}

char KernelFile::seek(unsigned long position)
{
	if (position > size) return '0';
	else {

		auto it = threadCursorMap.find(std::this_thread::get_id());
		if (it != threadCursorMap.end()) {
			it->second = position;
		}
		else {
			threadCursorMap.insert({ std::this_thread::get_id(), position });
		}

		cursor = position;
		return '1';
	}
}

char KernelFile::eof()
{
	if (filePos() == 0 && size == 0) return 2;

	if (filePos() == size) return 2;
	else return 0;
}


char KernelFile::truncate()
{
	//svaka nit koja cita ima svoj kursor
	auto it = threadCursorMap.find(std::this_thread::get_id());
	cursor = it->second;

	if (cursor == size) return 0;

	unsigned long dataClusterPoRedu = cursor / ClusterSize;
	unsigned long positionInDataCluster = cursor % ClusterSize;
	unsigned long index2ClusterPoRedu = dataClusterPoRedu / (ClusterSize / 4);

	unsigned long brojPodataka = size - cursor;

	unsigned long brDataClustKojeTrebaObrisati = 0;
	unsigned long brIndex2ClustKojeTrebaObrisati = 0;

	if (brojPodataka > ClusterSize) {
		if (brojPodataka % ClusterSize == 0) {
			brDataClustKojeTrebaObrisati = brojPodataka / ClusterSize;
		}
		else {
			brDataClustKojeTrebaObrisati = brojPodataka / ClusterSize + 1;
		}
	}

	if (brDataClustKojeTrebaObrisati > (ClusterSize / 4)) {
		if (brDataClustKojeTrebaObrisati % (ClusterSize / 4) == 0) brIndex2ClustKojeTrebaObrisati = brDataClustKojeTrebaObrisati / (ClusterSize / 4);
		else brIndex2ClustKojeTrebaObrisati = brDataClustKojeTrebaObrisati / (ClusterSize / 4) + 1;
	}

	partition->readCluster(getIndexOfIndex2Cluster(index2ClusterPoRedu), tmpIndex2Cluster); //dohvati mi index2Claster koji mi treba
	partition->readCluster(getIndexOfDataCluster(dataClusterPoRedu), tmpDataCluster); //dohvati mi dataClaster koji mi treba

	unsigned long brojac = 0;

	while (brojac < brojPodataka) {
		if (positionInDataCluster % ClusterSize == 0 && positionInDataCluster != 0) { //kada sam stigao do kraja data klastera
			if (dataClusterPoRedu == ClusterSize / 4 - 1) { //ako je slucajno taj data cluster 512. po redu (poslednji) => sledeci index2Cluster

				partition->writeCluster(getIndexOfIndex2Cluster(index2ClusterPoRedu), tmpIndex2Cluster);
				KernelFS::deallocateCluster(getIndexOfIndex2Cluster(index2ClusterPoRedu));
				brIndex2ClustKojeTrebaObrisati--;
				
				IndexCluster* ind1Cluster = (IndexCluster*)index1Cluster;
				ind1Cluster[index2ClusterPoRedu].entry = 0;
				index1FreeEntriesNum++;

				index2ClusterPoRedu++;
				index2FreeEntriesNum = ClusterSize / 4;
				partition->readCluster(getIndexOfIndex2Cluster(index2ClusterPoRedu), tmpIndex2Cluster);
				
			}
			
			partition->writeCluster(getIndexOfDataCluster(dataClusterPoRedu), tmpDataCluster);
			KernelFS::deallocateCluster(getIndexOfDataCluster(dataClusterPoRedu));
			brDataClustKojeTrebaObrisati--;
				
			IndexCluster* ind2Cluster = (IndexCluster*)tmpIndex2Cluster;
			ind2Cluster[dataClusterPoRedu].entry = 0;
			index2FreeEntriesNum++;

			if (dataClusterPoRedu == ClusterSize / 4 - 1) dataClusterPoRedu = 0;
			else dataClusterPoRedu++;

			dataFreeSpaceNum = ClusterSize;
			partition->readCluster(getIndexOfDataCluster(dataClusterPoRedu), tmpDataCluster);
			
		}
		tmpDataCluster[positionInDataCluster % ClusterSize] = 0;
		brojac++;
		positionInDataCluster++;
	}

	

	//proveriti da li ovaj index2 cluster treba da ostane => nije gotovo
	if (brIndex2ClustKojeTrebaObrisati == 1) { 
		KernelFS::deallocateCluster(getIndexOfIndex2Cluster(index2ClusterPoRedu));
		IndexCluster* ind1Cluster = (IndexCluster*)index1Cluster;
		ind1Cluster[index2ClusterPoRedu].entry = 0;
		brIndex2ClustKojeTrebaObrisati--;
		index1FreeEntriesNum++;
	}

	partition->writeCluster(getIndexOfDataCluster(dataClusterPoRedu), tmpDataCluster);
	KernelFS::deallocateCluster(getIndexOfDataCluster(dataClusterPoRedu));
	brDataClustKojeTrebaObrisati--;

	IndexCluster* ind2Cluster = (IndexCluster*)tmpIndex2Cluster;
	ind2Cluster[dataClusterPoRedu].entry = 0;
	index2FreeEntriesNum++; 
	
	partition->writeCluster(getIndexOfIndex2Cluster(index2ClusterPoRedu), tmpIndex2Cluster);
	partition->writeCluster(index1ClusterNumber, index1Cluster);

	if (cursor != 0) size = size - cursor;
	else size = 0;
	
	seek(size);

	return 1;
}


int KernelFile::getIndexOfIndex2Cluster(int entryNum) {
	IndexCluster* ind1Cluster = (IndexCluster*)index1Cluster;
	return ind1Cluster[entryNum].entry;
}

int KernelFile::getIndexOfDataCluster(int entryNum)
{
	IndexCluster* ind2Cluster = (IndexCluster*)tmpIndex2Cluster;
	return ind2Cluster[entryNum].entry;
}