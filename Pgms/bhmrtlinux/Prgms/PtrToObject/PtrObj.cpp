#include <stdio.h>
#include <iostream>
using namespace std;

class A
{
public:
	A() {printf("A()\n");i=0;}
	~A() {printf("~A()\n");}
	void func() {
		printf("&i = %p\n", &i);
		//cout<<"&i = "<<&i<<endl;
	}
private:
	int i;
};

int main()
{
	A* a = NULL;
	a->func();
	a = new A();
	a->func();
	delete a;
	a->func();
	a=NULL;
	a->func();
	a = (A*)10;
	a->func();
	return 0;
}

