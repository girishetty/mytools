#include <iostream>
using namespace std;

void ExceptionSample1(){
	try{
		int* pInt = new int (10);
		*pInt = 20;
		delete pInt;
	} catch(exception e) {
		cout<<"Caught some Exceptions"<<endl;
		//delete pInt;
	}
}

int main() {

	return 0;
}
