#pragma once

#include "KernelFile.h"

class File
{
public:
	File();
	File(Partition p, string name, int firstLvlIndex, int fSize, char mode='/');
private:
	KernelFile* kernelFile;
};

