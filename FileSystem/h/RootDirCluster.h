#pragma once

#include "DataStructures.h"
#include "part.h"
#include <string>
#include <iostream>

using namespace std;

//broj najtova koje zauzima svaki ulaz u root direktorijum
const unsigned long SizeOfRootDirEntry = 32;
//broj ulaza u root direktorijum
const unsigned long numOfEntriesInRootDir = ClusterSize / SizeOfRootDirEntry;

class RootDirCluster{

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

	/*
	* setuje velicinu fajla u bajtovima
	*/
	int setFileSize(int rootDirEntry, int fileSize = 0);

	/*
	* vraca velicinu fajla u bajtovima
	*/
	int getFileSize(int rootDirEntry);

private:
	DirEntry* dirEntry;
	Partition* partition;
	char buffer[2048];
};

