#include "DiamondClass.h"


/**********************************************************************
 * Function to make sure, vptr is the first entry in the class, if that
 * class has/inherits virtual function
 **********************************************************************/
void DoRnDoThisClass(){
	DerivedAA* pObj = new DerivedAA();
	long* pVPTR = (long*) pObj;
	long* pVTABLE = (long*) *pVPTR;
	cout<<"VPTR Address is : "<<pVPTR<<" VTABLE Address is : "<<pVTABLE<<endl;

	typedef void (*FUNPTR)();
	
	FUNPTR pFun = (FUNPTR) pVTABLE[1];
	pFun();
	pFun = (FUNPTR) pVTABLE[0];
	pFun();
	//Calling destructor this way wont harm anything.. It calls base destructor also
	pObj->~DerivedAA();
	//Calling destructor using this pointer CRASHES with ACCESS violation!!
	/*
	pFun = (FUNPTR) pVTABLE[2];
	pFun();
	*/
	delete pObj;
}


/**********************************************************************
 * Function which displays layout of plain class, class with virtual
 * functions, class with virtual functiona and inheritance and 
 * class with virtual inherits and virtual function
 **********************************************************************/
void SeeClassLayout(){
	cout<<"Non-virtual inheritance*******"<<endl;
	Derived d(20);
	cin.get();
	
	cout<<"Virtual inheritance without virtual function******"<<endl;
	cout<<"DerivedA:"<<endl;
	DerivedA dA(20);
	cout<<"DerivedB:"<<endl;
	DerivedB dB(20);
	cout<<"JoinAB:"<<endl;
	JoinAB jAB(20);
	cin.get();

	cout<<"Virtual inheritance with virtual function******"<<endl;
	DerivedAA dAA(20);
	DerivedBB dBB(20);
	JoinAABB jAABB(20);
	cin.get();
}

int main() {
	SeeClassLayout();
	//DoRnDoThisClass();

	return 0;
}
