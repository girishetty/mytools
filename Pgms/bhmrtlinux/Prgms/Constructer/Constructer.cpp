#include <iostream>
using namespace std;

class OneClass {
    int a;
public:
    OneClass (int val = 0) { 
        if(a>0)
            a+=val;
        else
            a=val;
    }
    void Set (int val) { a = val; }
    int Get () { return a; }
};

class TwoClass {
	OneClass one;
public:
    TwoClass (int val = 0) : one(val) {
	 //one.OneClass::OneClass(val+10); 
	 one.OneClass(val+10); 
    }
    void Set (int val) { one.Set (val); }
    int Get () { return one.Get (); }
};

int main () {
    TwoClass two(5);
    cout<<"Value is : "<<two.Get()<<endl;

    return 0;
}

