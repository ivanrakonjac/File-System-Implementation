#include "File.h"

File::File() {
	kernelFile = new KernelFile();
}

File::File(Partition p, string name, int firstLvlIndex, int fSize, char mode)
{
	kernelFile = new KernelFile(p, name, firstLvlIndex, fSize, mode);
}
