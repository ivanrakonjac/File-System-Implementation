#include <iostream>
#include <fstream>

#include "part.h"
#include "DataStructures.h"
#include "RootDirCluster.h"
#include "fs.h"
#include "BitVector.h"

using namespace std;



int main() {
	Partition* partition;

	char* ulazBuffer;
	int ulazSize;

	FILE* file = fopen("ulazMala.jpg", "rb");
	if (file == 0) {
		cout << "GRESKA: Nije nadjen ulazni fajl 'ulaz.dat' u os domacinu!" << endl;
		system("PAUSE");
		return 0;//exit program
	}
	ulazBuffer = new char[32 * 1024 * 1024];//32MB
	ulazSize = fread(ulazBuffer, 1, 32 * 1024 * 1024, file);
	fclose(file);

	partition = new Partition((char*)"resources/p1.ini");
	FS::mount(partition);
	FS::format();

	File* f;

	char filepath[] = "/fajl1.dat";
	f = FS::open(filepath, 'w');
	f->write(ulazSize, ulazBuffer);


	cout << "File size: " << f->getFileSize() << endl;

	File* src, * dst;
	char filepath1[] = "/fajll5.dat";
	dst = FS::open(filepath1, 'w');

	char c;

	f->seek(0);
	while (!f->eof()) {
		f->read(1, &c);
		dst->append(1, &c);
	}


	//delete dst;
	//delete src;

	ofstream fout("izlaz2.jpg", ios::out | ios::binary);
	char* buff = new char[f->getFileSize()];

	dst->seek(0);
	dst->truncate();

	f->seek(dst->getFileSize());
	while (!f->eof()) {
		f->read(1, &c);
		dst->append(1, &c);
	}

	dst->seek(0);
	dst->read(f->getFileSize(), buff);

	fout.write(buff, f->getFileSize());

	delete[] buff;
	fout.close();
	delete f;
}


