#ifndef _FOO_TEMPLATE_H
#define _FOO_TEMPLATE_H

#include <iostream>
using namespace std;

template <class T> class Foo {
public:
	Foo(T obj) : iT(obj) {}
	void Go() {
		cout<<iT<<endl;
	}
private:
	T iT;
};

#endif //_FOO_TEMPLATE_H