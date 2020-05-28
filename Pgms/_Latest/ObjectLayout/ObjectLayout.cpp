#include "Header.h"
#include "NonVirtual.h"
#include "Virtual.h"

int main() {
	/*
	cout<<"Hierarchy without virtual fun/with virtual inheritance"<<endl;
	CheckNonVirtualLayout();
	cin.get();
	cout<<"Hierarchy with virtual fun/with virtual inheritance"<<endl;
	*/
	//CheckVirtualLayout();
	//DisplaySize();
	cout<<endl<<"Embedded Object Alignment with in Object Layout"<<endl;
	ThisPointerAlignment();

	cout<<"Done"<<endl;
	return 0;
}
