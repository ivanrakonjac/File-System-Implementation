#pragma once

#include "part.h"
#include "RootDirCluster.h"
#include "BitVector.h"

#include <iterator> 
#include <map>

using namespace std;

class KernelFS {
public:

	static KernelFS* getInstance();
	static char mount(Partition* p);

	char doesFileExist(char* fname);

	long getNumberOfFiles();

	char format();

protected:

	static int initRootDirCluster(Partition* p);
	static int initBitVector(Partition* p);
	static int initFilesMap(); //inicijalizuje filesMap mapu postojecim fajlovima iz root direktorijuma (poziva se iz mounta)

private:
	static Partition* partition;
	static KernelFS* kernelFSInstace;

	static RootDirCluster* rootDirCluster;
	static BitVector* bitVector;
	static map<string, int> filesMap;

};