#pragma once

#include "DataStructures.h"
#include "fs.h"

using namespace std;

class KernelFile
{
public:
	KernelFile();
	KernelFile(Partition* p, string name, int firstLvlIndex, int fSize,char mode);

	char write(BytesCnt bytesCnt, char* buffer);

	char getMode() {
		return mode;
	}

	void setMode(char m) {
		mode = m;
	}


private:
	int numReaders;
	int numWriters;
	int numWaitRead;
	int numWaitWrite;
	mutex mutex;

	Partition* partition;
	string name;
	int firsLevelIndexClusterNumber;
	int size;
	char mode;
	int cursor;

	map< std::thread::id, int> threadCursorMap;
	
	char index1Cluster[ClusterSize];
};

