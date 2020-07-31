#include <iostream>
#include "part.h"

using namespace std;

int main() {

	Partition* p = new Partition((char*)"resources/p1.ini");

	char znak[2500];
	
	for (int i = 0; i < 2498; i++)
	{
		znak[i] = 't';

	}
	znak[2499] = '\0';

	cout << znak << endl;

	cout << "Vel znak[]:" << sizeof(znak) << endl;

	cout << "Write: " << p->writeCluster(0, znak) << endl;

	char* buffer = (char*)malloc(sizeof(char)*3000);
	cout << "Read:" << p->readCluster(0, buffer) << endl;

	int counter = 0;
	for (int i = 0; i < 3000; i++) {
		if (buffer[i] == 't') {
			counter++;
		}
	}
	
	cout << counter<<endl;
}