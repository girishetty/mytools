#include <iostream>
using namespace std;

int main() {
	char c;
	int* p = new int[10];
	*p = 0;
	int* p1 = p + 2;
	delete p1;
	cin>>c;
	return 0;
}
