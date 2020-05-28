

#include <iostream>
using namespace std;
#include <time.h>


int Global = 20;

int& foo() {
	int global = 10000;
	int& p = global; //Global;
	
	return p;
}

int main() {
	time_t curr_time;
	if(time(&curr_time) < 0 )  {
		printf("Time system call failed \n") ;
		return -1 ;
	}
	printf("Time value is %u \n" , curr_time) ;


	cout<<"Global before Foo is : "<<Global<<endl;
	int& val = foo();
	val = 200;
	cout<<"Global before Foo is : "<<Global<<endl;
	Global = 100;
	cout<<"Global before Foo is : "<<val<<endl;

	return 0;
}
