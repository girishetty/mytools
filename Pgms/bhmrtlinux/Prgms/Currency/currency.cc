#include <iostream>
using namespace std;

#define NO_OF_DENOMINATOR 8

class MyCurrency {
private:
	int bills[NO_OF_DENOMINATOR];
	int countb[NO_OF_DENOMINATOR];
	long total;

public:

	MyCurrency() {
		//Initialise denominators and initial counts
		bills[0] = 1000;
		bills[1] = 500;
		bills[2] = 100;
		bills[3] = 50;
		bills[4] = 20;
		bills[5] = 10;
		bills[6] = 5;
		bills[7] = 1;

		for(int i=0;i<NO_OF_DENOMINATOR;i++)
			countb[i] = 0;
	}

	void GiveChange() {
		int i,j;
		//Get the bill amount from the user
		cout<<"\nEnter the amount";
		cin>>total;
	
#if 0
		for(i=0;i<8;i++)
			for(j=0;(total/bills[i])>0;j++) {
				 countb[i]++;
				 total-=bills[i];
			}
#else
		//Get the count of each "defined" denominators
		for(i=0;i<8;i++) {
			countb[i] = total/bills[i];
			total -= (bills[i] * countb[i]);
		}
#endif
		//Give the details to the user
		cout<<"The denomination are\n";
		cout<<"Denomination --> No. of bills\n";

		for(i=0;i<8;i++)
			printf("%d    -->  %d\n",bills[i],countb[i]);
	}
};

int main() {
	MyCurrency c;
	c.GiveChange();
	return 0;
}

