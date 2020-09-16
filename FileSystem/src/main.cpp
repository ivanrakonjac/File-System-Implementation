#include <iostream>
#include "part.h"
#include "DataStructures.h"
#include "RootDirCluster.h"
#include "fs.h"

using namespace std;

int main() {

	Partition* p = new Partition((char*)"resources/p1.ini");
	
	cout << "Mount: " << FS::mount(p) << endl;

	char str[] = "testFile.c";
	cout << "Does file exist: " << FS::doesExist(str) <<endl;
	cout << "Broj fajlova u sistemu: " << FS::readRootDir()<<endl;

}