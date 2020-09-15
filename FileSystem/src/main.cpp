#include <iostream>
#include "part.h"
#include "DataStructures.h"
#include "RootDirCluster.h"
#include "fs.h"

using namespace std;

int main() {

	Partition* p = new Partition((char*)"resources/p1.ini");

	RootDirCluster* rdc = new RootDirCluster(p);
	cout<<"SetName: "<<rdc->setName(15,"ivan")<<endl;
	cout << "SetExtension: " << rdc->setEkstenzija(15, "cpp") << endl;
	cout << "SetFirstLevecIndexCluster: " << rdc->setFirsLevelIndexClusterNumber(15,255) << endl;
	cout << "GetFirstLevecIndexClusterNumber: " << rdc->getFirstLevelIndexClusterNumber(15) << endl;
	cout << "SetFileSize: " << rdc->setFileSize(15,128) << endl;
	cout << "GetFileSize: " << rdc->getFileSize(15) << endl;

	char buffer[2048];

	cout << "Read:" << p->readCluster(0, buffer) << endl;
	
	for (int i = 0; i < 2048; i++)
	{
		if (i % 32 == 0 && i > 0) cout << endl;
		cout << buffer[i];
	}

	cout << endl;

	cout << "Broj klastera: " << numOfEntriesInRootDir<< endl;

}