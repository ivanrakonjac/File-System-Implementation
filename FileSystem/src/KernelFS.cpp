#include "KernelFS.h"

KernelFS* KernelFS::kernelFSInstace = nullptr;
Partition* KernelFS::partition = nullptr;
RootDirCluster* KernelFS::rootDirCluster = nullptr;
BitVector* KernelFS::bitVector = nullptr;
map<string, int> KernelFS::filesMap;


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
;

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
			filesMap.insert({ fullFileName,12 });
		}
	}	

	/*for (auto itr = filesMap.begin(); itr != filesMap.end(); ++itr) {
		cout << '\t' << itr->first
			<< '\t' << itr->second << '\n';
	}
	cout << endl;*/

	return 1;
}


