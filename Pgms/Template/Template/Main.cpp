#include <iostream>
using namespace std;

#include "FooBag.h"

int main() {
	FooBag bag;
	Foo<int> intfoo(10);
	Foo<float> fltfoo = 20.5;
	Foo<char*> charfoo = "Girish";
	bag.Add(&intfoo);
	bag.Add(&fltfoo);
	bag.Add(&charfoo);
	return 0;
}
