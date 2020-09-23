#pragma once

#include "KernelFile.h"

class KernelFile;

class File
{
public:
	File(Partition* p, string name, int firstLvlIndex, int fSize, char mode='/');

	char write(unsigned long, char* buffer);
	char append(unsigned long bytesCnt, char* buffer);
	unsigned long read(unsigned long cnt, char* buffer);

	char seek(unsigned long position);
	unsigned long filePos();
	unsigned long getFileSize();
	char eof();
	char truncate();


	char deleteFile();

	

	void setMode(char m);
	char getMode();

	//dodaje kursor za nit koja pozove funckiju
	void addCursorForThread();

private:
	KernelFile* kernelFile;
};

