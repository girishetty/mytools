#ifndef CLASSDEF1_H
#define CLASSDEF1_H

#include <iostream>
using namespace std;

class Base {
        int iMem;
public:
        Base(int aMem = 0) : iMem(aMem) {
                cout<<"Base Constructed..."<<endl;
        }

        ~Base() {
                cout<<"Base Destructed..."<<endl;
        }
        void AddressOfBaseMem() const {
                cout<<"Address of Base::iMem is : "<<&iMem<<endl;
        }
};

class PlainDerived : public Base{
        int iMem;
public:
        PlainDerived(int aMem = 0) : Base(aMem), iMem(aMem+1) {
                cout<<"PlainDerived Constructed..."<<endl;
        }

        ~PlainDerived() {
                cout<<"PlainDerived Destructed..."<<endl;
        }
        void AddressOfPDerivedMem() const {
                AddressOfBaseMem();
                cout<<"Address of PlainDerived::iMem is : "<<&iMem<<endl;
        }
};

class Derived1 : virtual public Base{
        int iMem;
public:
        Derived1(int aMem = 0) : Base(aMem), iMem(aMem+1) {
                cout<<"Derived1 Constructed..."<<endl;
        }

        ~Derived1() {
                cout<<"Derived1 Destructed..."<<endl;
        }
        void AddressOfDerived1Mem() const {
                AddressOfBaseMem();
                cout<<"Address of Derived1::iMem is : "<<&iMem<<endl;
        }
};

class Derived2 : virtual public Base{
        int iMem;
public:
        Derived2(int aMem = 0) : Base(aMem), iMem(aMem+1) {
                cout<<"Derived2 Constructed..."<<endl;
        }

        ~Derived2() {
                cout<<"Derived2 Destructed..."<<endl;
        }
        void AddressOfDerived2Mem() const {
                AddressOfBaseMem();
                cout<<"Address of Derived2::iMem is : "<<&iMem<<endl;
        }
};


class Derived : public Derived1, public Derived2{
        int iMem;
public:
        Derived(int aMem = 0) : Base(aMem), Derived1(aMem+1),
                Derived2(aMem+2), iMem(aMem+3) {
                cout<<"Derived Constructed..."<<endl;
        }

        ~Derived() {
                cout<<"Derived Destructed..."<<endl;
        }
        void AddressOfDerivedMem() const {
                AddressOfDerived1Mem();
                AddressOfDerived2Mem();
                cout<<"Address of Derived::iMem is : "<<&iMem<<endl;
        }
};

void InheritanceLayout();
void VirtualInheritanceLayout();

#endif //CLASSDEF1_H

