#include <iostream>
#include "part.h"
#include "DataStructures.h"
#include "RootDirCluster.h"
#include "fs.h"
#include "BitVector.h"


std::mutex g_display_mutex;


using namespace std;

void foo()
{
	std::thread::id this_id = std::this_thread::get_id();

	g_display_mutex.lock();
	std::cout << "thread " << this_id << " u kriticnoj sekciji\n";
	std::this_thread::sleep_for(std::chrono::seconds(5));
	g_display_mutex.unlock();

}

void foo2()
{
	std::thread::id this_id = std::this_thread::get_id();

	g_display_mutex.lock();
	std::cout << "thread " << this_id << " u kriticnoj sekciji\n";
	g_display_mutex.unlock();

	std::this_thread::sleep_for(std::chrono::seconds(1));
}


int main() {

	Partition* p = new Partition((char*)"resources/p1.ini");
	FS::mount(p);
	FS::format();

	cout << "Br slob klastera: " << KernelFS::getNumOfFreeClusters() << endl;

	char filepath[] = "/proba.txt";
	File* file=FS::open(filepath, 'w');
	
	/*char filepath3[] = "/proba3.txt";
	FS::open(filepath3, 'w');

	char filepath4[] = "/proba4.txt";
	FS::open(filepath4, 'w');*/

	const unsigned long broj = 4100;
	char niz[broj];

	for (unsigned long i = 0; i < broj; i++) {
		niz[i] = 'i';
	}

	cout<<"Write: "<<file->write(broj, niz)<<endl;

	file = FS::open(filepath, 'r');
	char readNiz[broj];
	file->seek(0);

	cout << "READ NIZ:" << file->read(broj, readNiz) << endl;
	for (int i = 0; i < broj; i++) {
		if (i % 64 == 0 && i != 0) cout << endl;
		cout << readNiz[i];
	}
	cout << endl;

	file->seek(2048);
	cout << "SEEK NIZ:" << file->truncate() << endl;
	cout << "Cursor: " << file->filePos() << endl;
	cout << "Size: " << file->getFileSize() << endl;
	cout << "Mode: " << file->getMode() << endl;
	cout << "EOF: " << file->eof() << endl;
	
	cout << endl;

	char readNiz2[broj];
	file->seek(0);
	cout << "READ NIZ:" << file->read(broj, readNiz2) << endl;
	for (int i = 0; i < broj; i++) {
		if (i % 64 == 0 && i != 0) cout << endl;
		cout << readNiz2[i];
	}
	cout << endl;

	cout << "Cursor: " << file->filePos() << endl;
	cout << "Size: " << file->getFileSize() << endl;
	cout << "Mode: " << file->getMode() << endl;
	cout << "EOF: " << file->eof() << endl;

	cout << "Br slob klastera: " << KernelFS::getNumOfFreeClusters() << endl;

	return 0;

}