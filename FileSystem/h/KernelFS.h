#pragma once

#include "part.h"
#include "RootDirCluster.h"

#include <iostream> 

#include <iterator> 
#include <string>
#include <map>

using namespace std;

class KernelFS{
public:

	static KernelFS* getInstance();
	static char mount(Partition* p);

	char doesFileExist(char* fname);

	long getNumberOfFiles();

protected:

	static int initRootDirCluster(Partition* p);

	/*
	* inicijalizuje filesMap mapu postojecim fajlovima iz root direktorijuma
	* poziva se iz mounta
	*/
	static int initFilesMap();
	
private:
	static Partition* partition;
	static KernelFS* kernelFSInstace;
	
	static RootDirCluster* rootDirCluster;

	static map<string, int> filesMap;

};

