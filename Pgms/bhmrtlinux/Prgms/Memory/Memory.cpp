#include <iostream>
using namespace std;

int main() {
	int* p = new int;
	*p = 0;
	//int* p1 = p;
	char* p1 = (char*)p;
	for(int i =0; i<10; i++)
		cout<<*p1--<<endl;
	cin>>*p;
	return 0;
}

