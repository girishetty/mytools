#include <iostream>
#include <vector>

using namespace std;


int glob;
static int statglob;

#if GIRISH > 20

#endif
void food(long a) {
}

class A {
public:
    static int a;
};

int A::a = 20;

class B : public A {
};

enum DAY {
    MON = 1,
    TUE = 2
};

#define GIRISH = "SHETTY"

struct GIRI {
    int array_1[100];
};

int main() {
    const int size = 10;
    int val = 10.0;
    int pp = val/3.0;
    char ar[size];
    char* p = new char [100];
    cout<<"Hello"<<endl;
    char& qq = *p;
    //delete qq;
    int i = 20;
    food(i);
    DAY d = MON;
    DAY d1 = DAY::MON;
    DAY d2 = static_cast<enum DAY> (MON);

    int stackVar=0;
    stackVar+=1;

    cout<<statglob<<" "<<glob<<" "<<stackVar<<endl;

    B b;
    b.a = 20;
    B::a = 10;
    double dd = 1;

    if(1);
    cout<<"Girish"<<endl;
    if(0);
    cout<<"Shetty"<<endl;

//    cout<<"sizeof(array_1[100]): "<<sizeof(GIRI.array_1[100])<<" sizeof(int[100]): "<<sizeof(int[100])<<" sizeof(array_1): "<<sizeof(GIRI.array_1)<<endl;
    //cout<<" sizeof(array_1): "<<sizeof(GIRI::array_1)<<endl;
        

    return 0;
}
