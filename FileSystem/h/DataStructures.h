#pragma once

#include "part.h"
#include <stdint.h>
#include <string>
#include <iostream>


//broj bajtova koje zauzima svaki ulaz u root direktorijum
const unsigned long SizeOfRootDirEntry = 32;
//broj ulaza u root direktorijum
const unsigned long numOfEntriesInRootDir = ClusterSize / SizeOfRootDirEntry;

class DirEntry {
public:
	char fileName[8] = { 0 };
	char fileExtension[3] = { 0 };
	char notUsed = 0;
	uint32_t firstLevelIndexCluster;
	uint32_t fileSize;
	char free[12] = { 0 };
};

