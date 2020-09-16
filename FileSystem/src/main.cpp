#include <iostream>
#include "part.h"
#include "DataStructures.h"
#include "RootDirCluster.h"
#include "fs.h"

using namespace std;

int main() {

	Partition* p = new Partition((char*)"resources/p1.ini");
	
	cout << "Mount: " << FS::mount(p) << endl;

	char str[] = "testFile1.md";
	cout << "Does file exist: " << FS::doesExist(str);

}