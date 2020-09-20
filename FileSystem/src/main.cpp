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

	char filepath[] = "/proba.txt";
	FS::open(filepath, 'w');

	char filepath2[] = "/proba2.txt";
	FS::open(filepath2, 'w');
	
	char filepath3[] = "/proba3.txt";
	FS::open(filepath3, 'w');

	char filepath4[] = "/proba4.txt";
	FS::open(filepath4, 'w');

	/*RootDirCluster* rdc = new RootDirCluster(p);
	cout << "SetName: " << rdc->setName(2, "ivanFajl") << endl;
	cout << "SetExtension: " << rdc->setEkstenzija(2, "txt") << endl;
	cout << "SetFirstLevecIndexCluster: " << rdc->setFirsLevelIndexClusterNumber(2, 10025) << endl;
	cout << "GetFirstLevecIndexClusterNumber: " << rdc->getFirstLevelIndexClusterNumber(2) << endl;
	cout << "SetFileSize: " << rdc->setFileSize(2, 721) << endl;
	cout << "GetFileSize: " << rdc->getFileSize(2) << endl;*/

	char buffer[2048];

	cout << "Read cluster1: " << p->readCluster(1, buffer) << endl;

	for (int i = 0; i < 2048; i++) {
		if (i % 32 == 0 && i!= 0) cout << endl;
		cout << buffer[i];
	}

	cout << endl;

	char buffer2[2048];

	cout << "Read cluster0: " << p->readCluster(0, buffer2) << endl;

	for (int i = 0; i < 2048; i++) {
		if (i % 8 == 0 && i != 0) cout << endl;
		cout << buffer2[i];
	}


	/*std::thread t1(foo);
	std::thread t2(foo);
	std::thread t3(foo);
	std::thread t4(foo2);

	t1.join();
	t2.join();
	t3.join();
	t4.join();*/


	return 0;

}