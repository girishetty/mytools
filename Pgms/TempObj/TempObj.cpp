#include <iostream>
using namespace std;

class Number {
        int iNum;
        static int iObjCount;
        int iObjId;

public:
        Number(int aNum = 0) : iNum(aNum), iObjId(iObjCount++) {
                cout<<"Number::Number for Object "<<iObjId<<endl;
        }
        ~Number(){
                cout<<"Number::~Number for Object "<<iObjId<<endl;
        }

        Number operator+(Number& aNumber) {
                return Number(iNum + aNumber.iNum);
        }

        int Value() const {
                return iNum;
        }
};
int Number::iObjCount = 1;

void TempObjOnClass(){
        Number n1(10);
        Number n2(20);
        cout<<"Adding 2 Numbers gave : "<<(n1+n2).Value()<<endl;
        //Number& n = n1+n2; //This is also an error
        const Number& n3 = n1+n2;
        cout<<"New Number Ref Has : "<<n3.Value()<<endl;
}

void TempObjOnBasicType(){
        //int& i1 = 1; //This is an ERROR
        const int& i2 = 1;
}

int main() {
        TempObjOnBasicType();
        TempObjOnClass();
        return 0;
}


                                                                    