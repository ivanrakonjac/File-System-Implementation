#include "part.h"
#include "fs.h"
#include "KernelFS.h"

#include <iostream>

using namespace std;

KernelFS* FS::myImpl = nullptr;

char FS::mount(Partition* partition)
{
	FS();
	return myImpl->mount(partition);
}
/*
char FS::unmount()
{
	//return myImpl->unmount();
}*/
char FS::format()
{
	return myImpl->format();
}

FileCnt FS::readRootDir()
{
	return myImpl->getNumberOfFiles();
}

char FS::doesExist(char* fname)
{
	return myImpl->doesFileExist(fname);
}

// ReSharper disable once CppParameterMayBeConst
File* FS::open(char* fname, char mode)
{
	return myImpl->open(fname, mode);
}
/*
char FS::deleteFile(char* fname)
{
	//return myImpl->delete_file(fname);
}*/

FS::FS() {
	myImpl = KernelFS::getInstance();
}

FS::~FS()
{
	// empty body
}