#pragma once

#include "DataStructures.h"

using namespace std;

class RootDirCluster {

public:
	RootDirCluster(Partition* p);

	/*
	* funckija koja setuje prvih 8 bita u ulazu root direktorijuma
	* ako ime ima manje od 8b dopunjuje se ' '
	*/
	int setName(int rootDirEntry, string name);

	/*
	* setuje 9, 10 i 11 bit u ulazu root direktorijuma
	* to je ekstenzija fajla
	*/
	int setEkstenzija(int rootDirEntry, string ekstenzija);

	int setFirsLevelIndexClusterNumber(int rootDirEntry, int number = 0);

	int getFirstLevelIndexClusterNumber(int rootDirEntry);

	//setuje velicinu fajla u bajtovima
	int setFileSize(int rootDirEntry, int fileSize = 0);

	//vraca velicinu fajla u bajtovima
	int getFileSize(int rootDirEntry);

	//vraca imeFajla.ekstenzija
	string getFullFileName(int rootDirEntry);
	
	char* getFullFileNameChar(int rootDirEntry);

	//setuje sve ulaze na 0;
	void format();

	//vraca broj slobodnih ulaza u rootDir
	int getNumOfFreeEntries() {
		return numOfFreeEntries;
	}

	void incNumOfFreeEntries() {
		numOfFreeEntries++;
	}

	void decNumOfFreeEntries() {
		numOfFreeEntries--;
	}

	//vraca index prvog slobodnog ulaza ili -1, ako nema slobodnih
	int getIndexOfFreeEntry();

	//cuva buffer u cluster 1
	int saveRootDirClusterOnDisk() {
		return partition->writeCluster(1, buffer);
	}

private:
	DirEntry* dirEntry;
	Partition* partition;
	char buffer[2048];

	int numOfFreeEntries;
};