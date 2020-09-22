#pragma once

#include "part.h"
#include "RootDirCluster.h"
#include "BitVector.h"
#include "File.h"

#include <iterator> 
#include <map>

using namespace std;

class File;

class KernelFS {
public:

	static KernelFS* getInstance();
	static char mount(Partition* p);

	char doesFileExist(char* fname);

	long getNumberOfFiles();

	char format();

	File* open(char* fName, char mode);

	/*
	* alocira slobodan cluster
	* vraca index clustera ako uspe ili -1 ako nema mesta
	*/
	static int allocateCluster();

	static int deallocateCluster(int cluserIndex);

	static unsigned long getNumOfFreeClusters() {
		return bitVector->getNumOfFreeClusters();
	}

protected:

	static int initRootDirCluster(Partition* p);
	static int initBitVector(Partition* p);
	static int initFilesMap(); //inicijalizuje filesMap mapu postojecim fajlovima iz root direktorijuma (poziva se iz mounta)

	File* openRead(char* fName);
	File* openWrite(char* fName);

	string getNameFromFullName(char* fName);
	string getExtensionFromFullName(char* fullFileName);

private:
	static Partition* partition;
	static KernelFS* kernelFSInstace;

	static RootDirCluster* rootDirCluster;
	static BitVector* bitVector;
	static map<string, File*> filesMap;

};