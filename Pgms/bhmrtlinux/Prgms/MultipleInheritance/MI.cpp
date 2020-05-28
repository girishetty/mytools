#include <iostream>
using namespace std;

#define VIRTUALLY /*virtual*/
#define VIRTUAL /*virtual*/

class BaseA {
	int iMem;

public:
	BaseA(int aVal = 0) : iMem(aVal) {
		ClassAddressInfo();
	}

	VIRTUAL void ClassAddressInfo() const {
		long* pVPTR= (long*) this;
		long* pVTBL = (long*) *pVPTR;
		cout<<endl<<"Info on BaseA ...."<<endl;
		cout<<"Size is : "<<sizeof(BaseA)<<endl;
		cout<<"Address of VPTR is : "<<pVPTR<<endl;
		cout<<"Address of VTABLE is : "<<pVTBL<<endl;
		cout<<"Address of BaseA::iMem is : "<<&iMem<<endl;
	}

	VIRTUAL ~BaseA() {
		cout<<"Destructing BaseA"<<endl;
	}
};

class BaseB {
	int iMem;

public:
	BaseB(int aVal = 0) : iMem(aVal) {
		ClassAddressInfo();
	}

	VIRTUAL void ClassAddressInfo() const {
		long* pVPTR= (long*) this;
		long* pVTBL = (long*) *pVPTR;
		cout<<endl<<"Info on BaseB ...."<<endl;
		cout<<"Size is : "<<sizeof(BaseB)<<endl;
		cout<<"Address of VPTR is : "<<pVPTR<<endl;
		cout<<"Address of VTABLE is : "<<pVTBL<<endl;
		cout<<"Address of BaseB::iMem is : "<<&iMem<<endl;
	}

	VIRTUAL ~BaseB() {
		cout<<"Destructing BaseB"<<endl;
	}
};

class BaseC {
	int iMem;

public:
	BaseC(int aVal = 0) : iMem(aVal) {
		ClassAddressInfo();
	}

 VIRTUAL void ClassAddressInfo() const {
		long* pVPTR= (long*) this;
		long* pVTBL = (long*) *pVPTR;
		cout<<endl<<"Info on BaseC ...."<<endl;
		cout<<"Size is : "<<sizeof(BaseC)<<endl;
		cout<<"Address of VPTR is : "<<pVPTR<<endl;
		cout<<"Address of VTABLE is : "<<pVTBL<<endl;
		cout<<"Address of BaseC::iMem is : "<<&iMem<<endl;
	}

	VIRTUAL ~BaseC() {
		cout<<"Destructing BaseC"<<endl;
	}
};

class MI : VIRTUALLY public BaseA, VIRTUALLY virtual public BaseB, VIRTUALLY public BaseC{
	int iMem;

public:
	MI(int aVal = 0) : BaseA(aVal), BaseB(aVal+1), BaseC(aVal+2), iMem(aVal+3) {
		ClassAddressInfo();
	}

	VIRTUAL void ClassAddressInfo() const {
		long* pVPTR= (long*) this;
		long* pVTBL = (long*) *pVPTR;
		cout<<endl<<"Info on MI ...."<<endl;
		cout<<"Size is : "<<sizeof(MI)<<endl;
		cout<<"Address of VPTR is : "<<pVPTR<<endl;
		cout<<"Address of VTABLE is : "<<pVTBL<<endl;
		cout<<"Address of MI::iMem is : "<<&iMem<<endl;
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

	//Below 4 statements does samething when destructor is virtual
	//delete pA;
	//delete pB;
	//delete pC;
	delete pMI;
}

int main(){
	//MultipleInheritanceTest1();
	MultipleInheritanceTest2();
	return 0;
}


