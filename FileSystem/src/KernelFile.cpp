#include "KernelFile.h"

KernelFile::KernelFile()
{
	numReaders = 0;
	numWriters = 0;
	numWaitRead = 0;
	numWaitWrite = 0;

	name = "";
	firsLevelIndexClusterNumber = 0;
	size = 0;
	mode = '/';
	cursor = 0;
}

KernelFile::KernelFile(Partition* p, string fName, int firstLvlIndex, int fSize, char m)
{
	numReaders = 0;
	numWriters = 0;
	numWaitRead = 0;
	numWaitWrite = 0;

	partition = p;
	name = fName;
	firsLevelIndexClusterNumber = firstLvlIndex;
	size = fSize;
	mode = m;
	cursor = 0;

	threadCursorMap.insert({ std::this_thread::get_id(), 0 });

	partition->writeCluster(firsLevelIndexClusterNumber, index1Cluster);

	cout << "New file: " << name << " index1=" << firsLevelIndexClusterNumber << " size=" << size << " mode=" << m <<endl;

	for (auto itr = threadCursorMap.begin(); itr != threadCursorMap.end(); ++itr) {
		cout << '\t' << itr->first
			<< '\t' << itr->second << '\n';
	}
	cout << endl;
}

char KernelFile::write(BytesCnt bytesCnt, char* buffer)
{
	int dataClusterNum = 0;
	int index2ClusterNum = 0;

	if (size % ClusterSize == 0) dataClusterNum = size / ClusterSize;
	else dataClusterNum = size / ClusterSize;

	if (dataClusterNum % (ClusterSize / 32) == 0) index2ClusterNum = dataClusterNum / (ClusterSize / 32);
	else index2ClusterNum = dataClusterNum / (ClusterSize / 32) + 1;



	int index2Cluster = index1Cluster[index2ClusterNum];


	return 0;
}
