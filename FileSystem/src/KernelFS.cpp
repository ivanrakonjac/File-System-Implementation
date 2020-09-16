#include "KernelFS.h"

KernelFS* KernelFS::kernelFSInstace = nullptr;
Partition* KernelFS::partition = nullptr;
RootDirCluster* KernelFS::rootDirCluster = nullptr;
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
	initFilesMap();

	return '1';
}
char KernelFS::doesFileExist(char* fname)
{
	auto it = filesMap.find(fname);

	if (it != filesMap.end()) return '1';
	else return '0';
}
;

int KernelFS::initRootDirCluster(Partition* p) {
	KernelFS::rootDirCluster = new RootDirCluster(p);
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

	cout << "SVI ELEMENTI"<<endl;
	for (auto itr = filesMap.begin(); itr != filesMap.end(); ++itr) {
		cout << itr->first
			<< '\t' << itr->second << '\n';
	}
	
	return 1;
}


