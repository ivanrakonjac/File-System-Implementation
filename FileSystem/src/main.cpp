#include <iostream>
#include "part.h"
#include "DataStructures.h"
#include "RootDirCluster.h"
#include "fs.h"
#include "BitVector.h"


using namespace std;

int main() {

	Partition* p = new Partition((char*)"resources/p1.ini");

	cout << "Mount: " << FS::mount(p) << endl;
	cout << "Format: " << FS::format() << endl;
	cout << "Read root dir: " << FS::readRootDir() << endl;
	char imeFajla[] = "testFile.txt";
	cout << "Does exist: " << FS::doesExist(imeFajla) << endl;


	/*bitVector->setClusterBit(0);
	bitVector->setClusterBit(1);
	bitVector->setClusterBit(2);
	bitVector->setClusterBit(3);
	bitVector->setClusterBit(4);
	bitVector->setClusterBit(5);
	bitVector->setClusterBit(6);
	bitVector->setClusterBit(7);
	bitVector->setClusterBit(8);

	cout << "First free: " << bitVector->getIndexOfFreeCluster() << endl;

	bitVector->clearClusterBit(8);
	bitVector->clearClusterBit(7);
	bitVector->clearClusterBit(6);
	bitVector->clearClusterBit(5);
	bitVector->clearClusterBit(4);
	bitVector->clearClusterBit(3);
	bitVector->clearClusterBit(2);
	bitVector->clearClusterBit(1);

	for (int i = 2; i < 8000; i++) {
		if (!bitVector->isClusterFree(i)) {
			bitVector->setClusterBit(i);
			bitVector->incNumFreeClusters();
		}
	}

	cout << "isFree 0: " << bitVector->isClusterFree(10)<<endl;
	cout << "isFree 1: " << bitVector->isClusterFree(150) << endl;
	cout << "num of free clusters: " << bitVector->freeClusters()<<endl;
	cout << "num of clusters: " << bitVector->getNumOfCluster()<<endl;
	cout << "First free: " << bitVector->getIndexOfFreeCluster() << endl;


	for (int i = 4500; i < 9000; i++) {
		if (bitVector->isClusterFree(i)) {
			bitVector->setClusterBit(i);
			bitVector->decNumFreeClusters();
		}
	}

	cout << "isFree 10: " << bitVector->isClusterFree(10) << endl;
	cout << "isFree 4600: " << bitVector->isClusterFree(4600) << endl;
	cout << "num of free clusters: " << bitVector->freeClusters() << endl;
	cout << "First free: " << bitVector->getIndexOfFreeCluster() << endl;

	bitVector->format();

	cout << "isFree 10: " << bitVector->isClusterFree(10) << endl;
	cout << "isFree 150: " << bitVector->isClusterFree(150) << endl;
	cout << "num of free clusters: " << bitVector->freeClusters() << endl;
	cout << "First free: " << bitVector->getIndexOfFreeCluster()<<endl;*/

}