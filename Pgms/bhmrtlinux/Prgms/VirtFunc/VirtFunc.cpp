#include <cstdlib>
#include <iostream>

using namespace std;

typedef void(*FunctionPrint)();


//#define COMMENT_1
#define COMMENT_2


class A {
public:
    A() : m_nA (1) {
		cout<<"A::A()"<<endl;
	}

    virtual void Print(){
		cout<<"A::Print() : "<<m_nA<<endl;
	}

protected:
    int m_nA;
};

class B: public A {
public:
    B() : m_nB(2) {
		cout<<"B::B()"<<endl;
	}

    virtual void Print(){
		cout<<"B::Print() : "<<m_nB<<endl;
	}
    void Func1(){
#ifdef COMMENT_1
		cout<<"B::Func1()"<<endl;
#endif //COMMENT_1
	}
private:
    int m_nB;

};

/*
 * Try commenting either #define COMMENT_1 or #define COMMENT_2 or both.
 * you will get 3 different outputs !!
 */
int main() {
	B* pB = new B();
    long *pClass = (long *)( pB);   // point to class
    long *pVptr  =  (long *)(* pClass);      // point to vptr
    long *pPrint = (long *)(* pVptr);      // point to print function

    FunctionPrint MyPrint = (FunctionPrint)pPrint;

#ifdef COMMENT_2
    pB->Func1(); //Line1 
	//if i uncomment above line, MyPrint() and pB->Print() will get the same results! why?
#endif //COMMENT_2
    MyPrint();  //the result depends on Line1
    pB->Print();

	delete pB;
    //system("Pause");
    return EXIT_SUCCESS;
}

