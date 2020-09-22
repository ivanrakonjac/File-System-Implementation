#include "KernelFS.h"

KernelFS* KernelFS::kernelFSInstace = nullptr;
Partition* KernelFS::partition = nullptr;
RootDirCluster* KernelFS::rootDirCluster = nullptr;
BitVector* KernelFS::bitVector = nullptr;
map<string, File*> KernelFS::filesMap;


KernelFS* KernelFS::getInstance() {
	if (kernelFSInstace == nullptr) {
		kernelFSInstace = new KernelFS();
	}
	return kernelFSInstace;
}

char KernelFS::mount(Partition* p) {
	if (p == nullptr) return '0';
	partition = p;

	initRootDirCluster(partition);
	initBitVector(partition);
	initFilesMap();

	return '1';
}

char KernelFS::doesFileExist(char* fname)
{
	auto it = filesMap.find(fname);

	if (it != filesMap.end()) return '1';
	else return '0';
}

long KernelFS::getNumberOfFiles()
{
	long number = filesMap.size();
	if (number > 0) return number;
	else return -1;
}

char KernelFS::format()
{
	if (filesMap.size() > 0) {
		filesMap.clear();
	}
	rootDirCluster->format();
	bitVector->format();

	return '1';
}

File* KernelFS::open(char* fName, char mode)
{
	if (partition == nullptr) return nullptr;

	//uklanjanje kose crte iz apsolutne putanje
	char fileName[14] = { 0 };
	int i = 1;
	for (; i < strlen(fName); i++) fileName[i - 1] = fName[i];
	fileName[i] = '\0';

	char fileExists = doesFileExist(fileName);
	File* newFile = nullptr;

	switch (mode)
	{
	case 'r':
		if (fileExists == '1') {
			return openRead(fileName); 	
		}
		return nullptr;
	case 'w':
		if (fileExists == '1') {
			//obrisi fajl
			cout << "fajlObrisan" <<endl;
		}
		
		newFile = openWrite(fileName);
		filesMap.insert({ fileName,newFile});
		return newFile;

		break;
	case 'a':
		cout << "mode: a | doesFileExist: " << fileExists << endl;
		break;
	default:
		break;
	}
	return nullptr;
}

File* KernelFS::openRead(char* fName) {
	auto it = filesMap.find(getNameFromFullName(fName)+"."+getExtensionFromFullName(fName));
	
	File* file = it->second;
	file->setMode('r');
	file->addCursorForThread();

	return file;
}

File* KernelFS::openWrite(char* fName) {
	cout << "openWrite: " << fName << endl;

	if (rootDirCluster->getNumOfFreeEntries() > 0 && bitVector->getNumOfFreeClusters()>1) {
		
		int rootDirEntry = rootDirCluster->getIndexOfFreeEntry();
		rootDirCluster->decNumOfFreeEntries();

		int index1 = bitVector->getIndexOfFreeCluster();
		bitVector->clearClusterBit(index1);
		bitVector->decNumFreeClusters();

		rootDirCluster->setName(rootDirEntry, getNameFromFullName(fName));
		rootDirCluster->setEkstenzija(rootDirEntry, getExtensionFromFullName(fName));
		rootDirCluster->setFirsLevelIndexClusterNumber(rootDirEntry, index1);
		rootDirCluster->setFileSize(rootDirEntry, 0);

		cout<<"RootDirOnDisk: "<<rootDirCluster->saveRootDirClusterOnDisk()<<endl;
		cout << "BitVectorOnDisk: " << bitVector->saveBitVectorOnDisk() << endl;

		return new File(partition, rootDirCluster->getFullFileName(rootDirEntry), index1, 0,'w');

	}
	else {
		return nullptr;
	}

	cout << "Free entry: " << rootDirCluster->getIndexOfFreeEntry()<<endl;
	return nullptr;
}


int KernelFS::initRootDirCluster(Partition* p) {
	KernelFS::rootDirCluster = new RootDirCluster(p);
	return 1;
}

int KernelFS::initBitVector(Partition* p)
{
	KernelFS::bitVector = new BitVector(p);
	return 1;
}

int KernelFS::initFilesMap() {
	for (int i = 0; i < numOfEntriesInRootDir; i++)
	{
		string fullFileName = rootDirCluster->getFullFileName(i);
		if (fullFileName != "-1") {
			filesMap.insert({ fullFileName, nullptr});
		}
		//new File(fullFileName,rootDirCluster->getFirstLevelIndexClusterNumber(i),rootDirCluster->getFileSize(i))
	}	

	cout << "Mapa fajlova" << endl;
	for (auto itr = filesMap.begin(); itr != filesMap.end(); ++itr) {
		cout << '\t' << itr->first
			<< '\t' << itr->second << '\n';
	}
	cout << endl;

	return 1;
}

string KernelFS::getNameFromFullName(char* fullFileName) {
	std::string fName(fullFileName);
	std::string delimiter = ".";

	size_t pos = 0;
	std::string name;
	pos = fName.find(delimiter);
	name = fName.substr(0, pos);

	return name;
}

string KernelFS::getExtensionFromFullName(char* fullFileName) {
	std::string fName(fullFileName);
	std::string delimiter = ".";

	size_t pos = 0;
	std::string name;
	pos = fName.find(delimiter);
	name = fName.substr(0, pos);
	fName.erase(0, pos + delimiter.length());

	return fName;
}

int KernelFS::allocateCluster() {
	int index = bitVector->getIndexOfFreeCluster();

	if (index > -1) {
		bitVector->clearClusterBit(index);
		bitVector->decNumFreeClusters();
		return index;
	}
	else {
		return -1;
	}	
}

int KernelFS::deallocateCluster(int clusterIndex) {
	bitVector->setClusterBit(clusterIndex);
	bitVector->incNumFreeClusters();
	return 1;
}


