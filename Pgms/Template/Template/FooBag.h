#ifndef _FOO_BAG_H
#define _FOO_BAG_H

#include <vector>
using namespace std;

#include "FooTemplate.h"

class FooGen : public Foo<void*> {
};

class FooBag {
public:
	template<class T> void Add(Foo<T>* obj) {
		FooGen* pObj = static_cast<FooGen*> obj;
		iList.add(pObj);
	}
	void Go() {
	}
private:
	vector<FooGen*> iList;
};

#endif //_FOO_BAG_H
