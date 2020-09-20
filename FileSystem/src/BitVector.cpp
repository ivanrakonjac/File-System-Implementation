#include "BitVector.h"

BitVector::BitVector(Partition* p)
{
	partition = p;
	p->readCluster(0, bitVector);

	clustersNum = p->getNumOfClusters();
	numFreeClusters = 0;

	for (int i = 0; i < clustersNum*8; i++)
	{
		if (isClusterFree(i)) numFreeClusters++;
	}
}

BitVector::~BitVector()
{
}

void BitVector::format()
{
	numFreeClusters = clustersNum * 8;
	
	for (int i = 0; i < ClusterSize; i++) {
		bitVector[i] = ~0;
	}

	clearClusterBit(0); 
	clearClusterBit(1);

	numFreeClusters -= 2;

	partition->writeCluster(0, bitVector);
}

int BitVector::freeClusters()
{
	return numFreeClusters;
}

void BitVector::setClusterBit(int clusterIndex)
{
	bitVector[(clusterIndex / 8)] |= (1 << (clusterIndex % 8));
}

void BitVector::clearClusterBit(int clusterIndex)
{
	bitVector[(clusterIndex / 8)] &= ~(1 << (clusterIndex % 8));
}

bool BitVector::isClusterFree(int clusterIndex)
{
	return bitVector[(clusterIndex / 8)] & (1 << (clusterIndex % 8));
}

int BitVector::getIndexOfFreeCluster()
{
	for (int i = 0; i < clustersNum * 8; i++) {
		if (isClusterFree(i)) return i;
	}

	return -1;
}

int BitVector::getNumOfClusters()
{
	return clustersNum;
}
