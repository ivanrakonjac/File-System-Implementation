#include "File.h"

File::File(Partition* p, string name, int firstLvlIndex, int fSize, char mode)
{
	kernelFile = new KernelFile(p, name, firstLvlIndex, fSize, mode);
}

File::~File()
{
	kernelFile->deleteFile();
}

char File::write(unsigned long cnt, char* buffer)
{
	if (kernelFile->getMode() == 'r') return 0;

	if (kernelFile->getMode() == 'a') {
		return kernelFile->append(cnt, buffer);
	}
	else {
		return kernelFile->write(cnt, buffer);
	}
}

char File::append(unsigned long bytesCnt, char* buffer)
{
	return kernelFile->append(bytesCnt, buffer);
}

unsigned long File::read(unsigned long cnt, char* buffer)
{
	return kernelFile->read(cnt,buffer);
}

void File::setMode(char m)
{
	kernelFile->setMode(m);
}

char File::getMode()
{
	return kernelFile->getMode();
}

void File::addCursorForThread()
{
	kernelFile->addCursorForThread();
}

char File::seek(unsigned long position)
{
	return kernelFile->seek(position);
}

unsigned long File::filePos()
{
	return kernelFile->filePos();
}

unsigned long File::getFileSize()
{
	return kernelFile->getFileSize();
}

char File::eof()
{
	return kernelFile->eof();
}

char File::truncate()
{
	return kernelFile->truncate();
}

char File::deleteFile()
{
	return kernelFile->deleteFile();
}
