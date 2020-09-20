#pragma once

#include "DataStructures.h"

using namespace std;

class BitVector
{
public:

	BitVector(Partition* p);
	~BitVector();

	void format();
	int freeClusters();

	void setClusterBit(int clusterIndex);
	void clearClusterBit(int clusterIndex);

	/*
	* 1 - slobodan;
	* 0 - zauzet;
	*/
	bool isClusterFree(int bitIndex);

	/*
	* vraca broj klastera na particiji
	*/
	int getNumOfClusters();

	/*
	* vraca index prvog slobodnog klaster
	* ako nema slobodnih vraca -1
	*/
	int getIndexOfFreeCluster();

	int getNumOfFreeClusters() {
		return numFreeClusters;
	}

	void incNumFreeClusters() {
		numFreeClusters++;
	}

	void decNumFreeClusters() {
		numFreeClusters--;
	}

	void decNumFreeClusters(int num) {
		numFreeClusters-=num;
	}

	int saveBitVectorOnDisk() {
		return partition->writeCluster(0, bitVector);
	}

private:
	Partition* partition;
	char bitVector[2048];
	int numFreeClusters;
	int clustersNum;

};

