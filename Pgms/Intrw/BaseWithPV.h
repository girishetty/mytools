#ifndef _BASE_WITH_PURE_VIRTUAL
#define _BASE_WITH_PURE_VIRTUAL

#include <iostream>
using namespace std;

#define DEFINED_PURE_VIRTUAL
//#define CRASH_ME

class Base {
private:
#ifdef DEFINED_PURE_VIRTUAL
    virtual void Cleanup() = 0 {
        cout<<"Base Cleanup Done"<<endl;
    }
#else
    virtual void Cleanup() = 0;
#endif //DEFINED_PURE_VIRTUAL

public:
    Base() {
        cout<<"Base Constructed"<<endl;
    }

    virtual ~Base() {
#ifdef CRASH_ME
        /*
         * Calling any function from constructor/destructor, which invokes pure virtual function 
         * will cause data abortion/segmentaion fault/crash
         * But calling any function from constructor/destructor, which invokes virtual function 
         * will invoke the virtual function in this (Base class) scope, as virtual table pointer will be reassigned
         */
        Clean();
#endif //CRASH_ME
#ifdef DEFINED_PURE_VIRTUAL
        /* 
         * unless we define pure virtual function in this (Base class) scope, 
         * we cant call pure virtual functions from both Constructor and Destructor (linker error)
         */
        Cleanup(); 
#endif //DEFINED_PURE_VIRTUAL
        CleanMe();
        cout<<"Base Destructed"<<endl;
    }

    virtual void CleanMe() {
        cout<<"Cleaning Base"<<endl;
    }

    void Clean() {
        cout<<"Cleaning..."<<endl;
        Cleanup();
    }
};

class Derived : public Base {
private:
    virtual void Cleanup() {
        delete iMem;
        iMem = 0;
        cout<<"Derived Cleanup Done"<<endl;
    }

    virtual void CleanMe() {
        cout<<"Cleaning Derived"<<endl;
    }

public:
    Derived() {
        iMem = new int;
        cout<<"Derived Constructed"<<endl;
    }

    ~Derived() {
        //call pure virtual first
        Cleanup();
        //call virtual next
        CleanMe();
        cout<<"Derived Destructed"<<endl;
    }

private:
    int* iMem;
};

#endif //_BASE_WITH_PURE_VIRTUAL
