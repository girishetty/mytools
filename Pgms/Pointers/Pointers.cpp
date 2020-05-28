# include <iostream>
using namespace std;

void PointerTest() {
	int array[2][3][4] ={
						{ {1,2,3,4},
						  {21,3,4,5},
						  {31,4,5,6},
						},
						{ {23,4,5,6},
						  {42,3,4,5},
						  {51,2,3,4},
						}
						};
	cout<<array<<endl;
	cout<<*array<<endl;
	cout<<**array<<endl;
	cout<<***array<<endl;

	cout<<array+1<<endl;
	cout<<*array+1<<endl;
	cout<<**array+1<<endl;
	cout<<***array+1<<endl;

	cout<<*(array+1)<<endl;
	cout<<**(array+1)<<endl;
	cout<<***(array+1)<<endl;

	cout<<(array+1-array)<<endl;
	cout<<(*array+1-*array)<<endl;
	cout<<(**array+1-**array)<<endl;
	cout<<(***array+1-***array)<<endl;

}

int main() {
	PointerTest();
	return 0;
}

