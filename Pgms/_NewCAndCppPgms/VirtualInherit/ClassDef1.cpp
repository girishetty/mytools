#include "ClassDef1.h"

void InheritanceLayout(){
        PlainDerived d;
        cout<<"Size of PlaninDerived is : "<<sizeof(PlainDerived)<<endl;
        cout<<"Address of PlainDerived is : "<<(&d)<<endl;
        d.AddressOfPDerivedMem();
}


void VirtualInheritanceLayout(){
        Base b;
        Derived1 d1;
        Derived2 d2;
        Derived d;
        cout<<"Size of All Class : "<<endl;
        cout<<"Size of Base is : "<<sizeof(Base)<<endl;
        cout<<"Size of Derived1 is : "<<sizeof(Derived1)<<endl;
        cout<<"Size of Derived2 is : "<<sizeof(Derived2)<<endl;
        cout<<"Size of Derived is : "<<sizeof(Derived)<<endl;
        cout<<endl<<"Address of All Class : "<<endl;
        cout<<"Address of Base is : "<<(&b)<<endl;
        cout<<"Address of Derived1 is : "<<(&d1)<<endl;
        cout<<"Address of Derived2 is : "<<(&d2)<<endl;
        cout<<"Address of Derived is : "<<(&d)<<endl;
        cout<<endl<<"First Member Address of All Class : "<<endl;
        b.AddressOfBaseMem();
        d1.AddressOfDerived1Mem();
        d2.AddressOfDerived2Mem();
        d.AddressOfDerivedMem();
}
