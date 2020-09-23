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

	FILE* file = fopen("Ulaz.jpg", "rb");
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

	/*char tmp[2048];
	partition->readCluster(295, tmp);
	cout << tmp<<endl;*/


	//f->deleteFile();

	//delete f;


	//char buffer[1500];

	//f->seek(0);
	//f->read(1500, buffer);

	cout << "File size: " << f->getFileSize() << endl;


	/*for (int i = 0; i < 1500; i++)
	{
		if (i % 20 == 0 && i != 0) cout << endl;
		cout << buffer[i];
	}*/

	//File* src, * dst;
	//char filepath2[] = "/fajl1.dat";
	/*src = FS::open(filepath2, 'r');
	src->seek(src->getFileSize() / 2);*/

	/*char filepath1[] = "/fajll5.dat";
	dst = FS::open(filepath1, 'w');

	char c;

	f->seek(0);
	f->read(1, &c);
	dst->write(1, &c);*/
	/*
	while (!f->eof()) {
		f->read(1, &c);
		//cout << c;
		dst->write(1, &c, 0);
	}

	cout << "f:" << f->filePos() << endl;
	cout << "dst:" << dst->filePos() << endl;

	cout << "position: " << dst->getFileSize() << endl;

	//delete dst;
	//delete src;
	*/
	//char filepath3[] = "/fajl5.dat";
	//f = FS::open(filepath1, 'r');

	ofstream fout("izlaz1.jpg", ios::out | ios::binary);
	char* buff = new char[f->getFileSize()];

	f->seek(0);
	f->read(f->getFileSize(), buff);

	

	/*for (int i = 0; i < f->getFileSize(); i++)
	{
		if (i % 20 == 0 && i != 0) cout << endl;
		cout << buff[i];
	}*/

	fout.write(buff, f->getFileSize());

	//delete[] buff;
	fout.close();
	delete f;
}


