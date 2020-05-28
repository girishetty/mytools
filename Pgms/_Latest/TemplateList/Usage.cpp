#include <iostream>
using namespace std;

#include "ListTemplate.h"

int main() {
	List<int>* myList = new List<int>;

	const int MaxNode = 10;
	Node<int>* nodes[MaxNode];

	for(int i=0;i<MaxNode; i++) {
		int val = i+1;
		nodes[i] = new Node<int>(val);
		myList->Add(nodes[i]);
	}

	myList->Display();
	delete myList;
	return 0;
}
