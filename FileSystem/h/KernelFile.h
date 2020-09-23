#pragma once

#include "DataStructures.h"
#include "fs.h"

using namespace std;

class KernelFile
{
public:

	KernelFile(Partition* p, string name, int firstLvlIndex, int fSize,char mode);

	char write(unsigned long bytesCnt, char* buffer);
	char write(unsigned long bytesCnt, char* buffer, unsigned long position);
	unsigned long read(unsigned long cnt, char* buffer);
	char append(unsigned long bytesCnt, char* buffer);

	char seek(unsigned long position);
	char eof();
	char truncate();

	unsigned long filePos() {
		return cursor;
	}
	unsigned long getFileSize() {
		return size;
	}

	char getMode() {
		return mode;
	}
	void setMode(char m) {
		mode = m;
	}


	

	char deleteFile();

	//dodaje kursor za nit koja pozove funckiju
	void addCursorForThread();

protected:

	//dohvata novi index2 claster, formatira ga i povezuje sa indexom1
	int getIndex2Cluster();

	//dohvata novi data claster, formatira ga i povezuje sa tempIndex2Clasterom
	int getDataCluster();

	//vraca index entryNumtog klastera u tempIndex2Clasteru
	int getIndexOfIndex2Cluster(int entryNum);
	
	//vraca index entryNumtog klastera u tempDataClasteru
	int getIndexOfDataCluster(int entryNum);

private:
	int numReaders;
	int numWriters;
	int numWaitRead;
	int numWaitWrite;
	mutex mutex;

	Partition* partition;
	string name;
	unsigned long size;
	char mode;
	int cursor;

	map< std::thread::id, int> threadCursorMap;
	
	int index1ClusterNumber;
	int index2ClusterNumber;
	int dataClusterNumber;

	int index1FreeEntriesNum;
	int index2FreeEntriesNum;
	int dataFreeSpaceNum;

	char index1Cluster[ClusterSize] = {};
	char tmpIndex2Cluster[ClusterSize] = {};
	char tmpDataCluster[ClusterSize] = {};
};



