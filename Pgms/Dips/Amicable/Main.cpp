#include "Amicable.h"

#include <iostream>
using namespace std;

int main() {

	int begin;
	int end;
	struct amicable* nos;

	cout<<"Enter the begin and end range : ";
	cin>>begin;
	cin>>end;

	nos = getAmicablePairs(begin, end);

	for(int i=0; i<nos->size;i++) {
		cout<<"["<<nos->numbers[i][0]<<", "<<nos->numbers[i][1]<<"]"<<endl;
	}

	free_amicable(nos);

	return 0;
}
