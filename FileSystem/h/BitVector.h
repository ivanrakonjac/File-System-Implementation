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
	* vraca index prvog slobodnog klaster
	*/
	int getIndexOfFreeCluster();

	int getNumOfCluster();

	void incNumFreeClusters() {
		numFreeClusters++;
	}

	void decNumFreeClusters() {
		numFreeClusters--;
	}

private:
	Partition* partition;
	char bitVector[2048];
	int numFreeClusters;
	int clustersNum;

};

