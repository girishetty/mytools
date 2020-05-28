#include <iostream>
using namespace std;

class Base {
        int iMem;
public:
        Base(int aMem = 0) : iMem(aMem) {
                ClassAddressInfo();
        }

        virtual ~Base() {
                cout<<"Base Destructed..."<<endl;
        }

        virtual void ClassAddressInfo() const {
                long* pVPTR= (long*) this;
                long* pVTBL = (long*) *pVPTR;
                cout<<endl<<"Info on Base ...."<<endl;
                cout<<"Size is : "<<sizeof(Base)<<endl;
                cout<<"Address of VPTR is : "<<pVPTR<<endl;
                cout<<"Address of VTABLE is : "<<pVTBL<<endl;
                cout<<"Address of Base::iMem is : "<<&iMem<<endl;
        }
};

class PlainDerived : public Base{
        int iMem;
public:
        PlainDerived(int aMem = 0) : Base(aMem), iMem(aMem+1) {
                ClassAddressInfo();
        }

        ~PlainDerived() {
                cout<<"PlainDerived Destructed..."<<endl;
        }

        virtual void ClassAddressInfo() const {
                long* pVPTR= (long*) this;
                long* pVTBL = (long*) *pVPTR;
                cout<<endl<<"Info on PlainDerived...."<<endl;
                cout<<"Size is : "<<sizeof(PlainDerived)<<endl;
                cout<<"Address of VPTR is : "<<pVPTR<<endl;
                cout<<"Address of VTABLE is : "<<pVTBL<<endl;
                cout<<"Address of PlainDerived::iMem is : "<<&iMem<<endl;
        }
};

class Derived1 : virtual public Base{
        int iMem;
public:
        Derived1(int aMem = 0) : Base(aMem), iMem(aMem+1) {
                ClassAddressInfo();
        }

        ~Derived1() {
                cout<<"Derived1 Destructed..."<<endl;
        }

        virtual void ClassAddressInfo() const {
                long* pVPTR= (long*) this;
                long* ptr = (long*) (*(pVPTR+8));
                long* pVTBL = (long*) *pVPTR;
                cout<<endl<<"Info on Derived1...."<<endl;
                cout<<"Size is : "<<sizeof(Derived1)<<endl;
                cout<<"Address of VPTR is : "<<pVPTR<<endl;
                cout<<"Address of VTABLE is : "<<pVTBL<<endl;
                cout<<"Address of Derived1::iMem is : "<<&iMem<<endl;
                cout<<"Address of VTABLE is : "<<(ptr)<<endl;
        }
};

class Derived2 : virtual public Base{
        int iMem;
public:
        Derived2(int aMem = 0) : Base(aMem), iMem(aMem+1) {
                ClassAddressInfo();
        }

        ~Derived2() {
                cout<<"Derived2 Destructed..."<<endl;
        }

        virtual void ClassAddressInfo() const {
                long* pVPTR= (long*) this;
                long* pVTBL = (long*) *pVPTR;
                cout<<endl<<"Info on Derived2...."<<endl;
                cout<<"Size is : "<<sizeof(Derived2)<<endl;
                cout<<"Address of VPTR is : "<<pVPTR<<endl;
                cout<<"Address of VTABLE is : "<<pVTBL<<endl;
                cout<<"Address of Derived2::iMem is : "<<&iMem<<endl;
        }
};


class Derived : public Derived1, public Derived2{
        int iMem;
public:
        Derived(int aMem = 0) : Base(aMem), Derived1(aMem+1),
                Derived2(aMem+2), iMem(aMem+3) {
                ClassAddressInfo();
        }

        ~Derived() {
                cout<<"Derived Destructed..."<<endl;
        }

        virtual void ClassAddressInfo() const {
                long* pVPTR= (long*) this;
                long* pVTBL = (long*) *pVPTR;
                cout<<endl<<"Info on Derived...."<<endl;
                cout<<"Size is : "<<sizeof(Derived)<<endl;
                cout<<"Address of VPTR is : "<<pVPTR<<endl;
                cout<<"Address of VTABLE is : "<<pVTBL<<endl;
                cout<<"Address of Derived::iMem is : "<<&iMem<<endl;
        }
};

void InheritanceLayout(){
        cout<<"Details of Plain Derived..."<<endl;

        PlainDerived d;
        cout<<"Size of PlaninDerived is : "<<sizeof(PlainDerived)<<endl;

        cout<<"Details of Virtual Derived..."<<endl;
        Derived1 d1;
        cout<<"Size of Derived1 is : "<<sizeof(Derived1)<<endl;
}

void VirtualInheritanceLayout(){
        Base b;
        Derived1 d1;
        Derived2 d2;
        Derived d;
}

int main() {
        InheritanceLayout();
        //VirtualInheritanceLayout();
        return 0;
}

