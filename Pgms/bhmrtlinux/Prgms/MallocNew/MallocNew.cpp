#include <stdlib.h>
#include <iostream>
using namespace std;

struct MyStruct {
	int  imem2;
	int* iMem1;
	MyStruct() {
		cout<<"MyStruct constructed"<<endl;
		iMem1 = new int;
	}
	~MyStruct() {
		cout<<"MyStruct getting deleted: iMem1 is :"<<iMem1<<endl;
		delete iMem1;
 		cout<<"MyStruct deleted successfully"<<endl;
	}
};

int main() {
	//int* p = (int*) malloc(sizeof(int));
	MyStruct* p = (MyStruct*) malloc(sizeof(MyStruct));
	cout<<"Before delete"<<endl;
	delete p;
	cout<<"After delete"<<endl;
	return 0;
}

