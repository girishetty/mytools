#include <iostream>
using namespace std;

#define VIRTUALLY virtual
#define VIRTUAL /*virtual*/

class BaseA {
	int iMemA;

public:
	BaseA(int aVal = 0) : iMemA(aVal) {
		cout<<"Constructing BaseA with "<<iMemA<<endl;
	}

	void MemberAddressA() const {
		cout<<"Address of BaseA::iMemA is : "<<&iMemA<<endl;
	}

	VIRTUAL ~BaseA() {
		cout<<"Destructing BaseA"<<endl;
	}
};

class BaseB {
	int iMemB;

public:
	BaseB(int aVal = 0) : iMemB(aVal) {
		cout<<"Constructing BaseB with "<<iMemB<<endl;
	}

	void MemberAddressB() const {
		cout<<"Address of BaseB::iMemB is : "<<&iMemB<<endl;
	}

	VIRTUAL ~BaseB() {
		cout<<"Destructing BaseB"<<endl;
	}
};

class BaseC {
	int iMemC;

public:
	BaseC(int aVal = 0) : iMemC(aVal) {
		cout<<"Constructing BaseC with "<<iMemC<<endl;
	}

	void MemberAddressC() const {
		cout<<"Address of BaseC::iMemC is : "<<&iMemC<<endl;
	}

	VIRTUAL ~BaseC() {
		cout<<"Destructing BaseC"<<endl;
	}
};

class MI : VIRTUALLY public BaseA, VIRTUALLY public BaseB, VIRTUALLY public BaseC{
	int iMemMI;

public:
	MI(int aVal = 0) : BaseA(aVal), BaseB(aVal+1), BaseC(aVal+2), iMemMI(aVal+3) {
		cout<<"Constructing MI with "<<iMemMI<<endl;
	}

	void MemberAddressMI() const {
		cout<<"Address of MI::iMemMI is : "<<&iMemMI<<endl;
	}

	VIRTUAL ~MI() {
		cout<<"Destructing MI"<<endl;
	}
};

void MultipleInheritanceTest1(){
	cout<<"Create MI with BaseA"<<endl;
	BaseA* pA = new MI;
	cout<<endl<<"Create MI with BaseB"<<endl;
	BaseB* pB = new MI;
	cout<<endl<<"Create MI with BaseC"<<endl;
	BaseC* pC = new MI;
	cout<<endl<<"Create MI with MI"<<endl;
	MI* pMI = new MI;

	//Below 4 statements does samething when destructor is virtual
	//If, destructors are not vit\rtual, then delete on pB and pC will CRASH
	delete pA;
	delete pB;
	delete pC;
	delete pMI;
}

void MultipleInheritanceTest2(){
	MI* pMI = new MI;
	BaseA* pA = pMI;
	BaseB* pB = pMI;
	BaseC* pC = pMI;

	cout<<"Pointer to MI using pA is "<<pA<<endl;
	cout<<"Pointer to MI using pB is "<<pB<<endl;
	cout<<"Pointer to MI using pC is "<<pC<<endl;
	cout<<"Pointer to MI using pMI is "<<pMI<<endl<<endl;

	pA->MemberAddressA();
	pB->MemberAddressB();
	pC->MemberAddressC();
	pMI->MemberAddressMI();

	//Below 4 statements does samething when destructor is virtual
	//delete pA;
	//delete pB;
	//delete pC;
	delete pMI;
}

void MultipleInheritanceTest3(){
	cout<<"Size of BaseA is : "<<sizeof(BaseA)<<endl;
	cout<<"Size of BaseB is : "<<sizeof(BaseB)<<endl;
	cout<<"Size of BaseC is : "<<sizeof(BaseC)<<endl;
	cout<<"Size of MI is : "<<sizeof(MI)<<endl;
}

int main(){
	//MultipleInheritanceTest1();
	MultipleInheritanceTest2();
	MultipleInheritanceTest3();
	return 0;
}

