#include <string.h>
#include <iostream>
using namespace std;

class Sample{
int aMem;
char* aName;

public:
        Sample(char* name, int a = 20) : aName(name), aMem(a) {
                cout<<"Sample::Sample with "<<aName<<"  "<<aMem<<endl;
        }

        Sample(const Sample& aSam) : aName(aSam.aName), aMem(aSam.aMem) {
                cout<<"Copy Const Sample::Sample with "<<aName<<"  "<<aMem<<endl;
        }

        ~Sample() {
                cout<<"Dest Sample "<<aName<<"  "<<aMem<<endl;
        }

		unsigned int operator strlen() {
			return 1;
		}
};


int main() {
		int s = strlen("abc");
        Sample a("a");
        Sample b(int val=40);
        Sample c("c", 10);
        Sample d = b(50);
        c = b();
        Sample e(d);
        return 0;
}

Sample b(int val) {
        cout<<"Begin fun b"<<endl;
        Sample obj("obj", val);
        cout<<"End fun b"<<endl;
        return obj;
}
