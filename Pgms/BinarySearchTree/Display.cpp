#include <iostream>
using namespace std;

void Display(int array[], int length) {
	cout<<"Array Contents :";
	for(int i = 0; i<length; i++) {
		cout<<" "<<array[i];
	}
	cout<<endl;
}
