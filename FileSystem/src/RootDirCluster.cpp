#include "RootDirCluster.h"

RootDirCluster::RootDirCluster(Partition* p) {
	partition = p;
	p->readCluster(0, buffer);
	dirEntry = (DirEntry*)buffer;
}

int RootDirCluster::setName(int rootDirEntry, string name) {

	if (name.length() == 0 || name.length() > 8 || rootDirEntry > numOfEntriesInRootDir - 1 || rootDirEntry < 0) return -1;

	for (int i = 0; i < name.length(); i++)
	{
		dirEntry[rootDirEntry].fileName[i] = name[i];
	}
	for (int j = name.length(); j < 8; j++)
	{
		dirEntry[rootDirEntry].fileName[j] = '*';
	}

	return 1;
}

int RootDirCluster::setEkstenzija(int rootDirEntry, string ekstenzija) {
	if (ekstenzija.length() > 3 || rootDirEntry > numOfEntriesInRootDir - 1 || rootDirEntry < 0) return -1;

	for (int i = 0; i < ekstenzija.length(); i++)
	{
		dirEntry[rootDirEntry].fileExtension[i] = ekstenzija[i];
	}

	for (int j = ekstenzija.length(); j < 3; j++)
	{
		dirEntry[rootDirEntry].fileExtension[j] = '*';
	}

	return 1;

}

int RootDirCluster::setFirsLevelIndexClusterNumber(int rootDirEntry, int number) {
	if (number < 0 || rootDirEntry > numOfEntriesInRootDir - 1 || rootDirEntry < 0) return -1;

	dirEntry[rootDirEntry].firstLevelIndexCluster = number;

	return 1;
}

int RootDirCluster::getFirstLevelIndexClusterNumber(int rootDirEntry) {
	return dirEntry[rootDirEntry].firstLevelIndexCluster;
}

int RootDirCluster::setFileSize(int rootDirEntry, int fileSize) {
	if (fileSize < 0 || rootDirEntry > numOfEntriesInRootDir - 1 || rootDirEntry < 0) return -1;

	dirEntry[rootDirEntry].fileSize = fileSize;

	cout << "Write: " << partition->writeCluster(0, buffer) << endl;

	return 1;
}

int RootDirCluster::getFileSize(int rootDirEntry) {
	return dirEntry[rootDirEntry].fileSize;
}
