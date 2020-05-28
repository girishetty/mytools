#ifndef LISTTEMPLATE_H
#define LISTTEMPLATE_H

#include <iostream>
using namespace std;

const int SUCCESS = 0;
const int FAILED = -1;

template <class Data> struct Node {
	inline Node(Data& aData, Node* aLink = NULL) : iData(aData), iNext(aLink) {
	}

	inline void Display() {
		cout<<iData<<" ";
	}

	Data  iData;
	Node* iNext;
};

template <class Data> class List {
public:
	inline List() : iHead(NULL), iCurrent(NULL) {
		int arg = 0;
		iHead = iCurrent = new Node<Data>(arg);
	}

	~List() {
		while(iHead){
			iCurrent = iHead->iNext;
			delete iHead;
			iHead = iCurrent;
		}
	}

	void Display() {
		Node<Data>* p = iHead->iNext;
		cout<<"List Starts : ";
		while(p){
			p->Display();
			p = p->iNext;
		}
		cout<<"List Ends"<<endl;
	}

	//Inserts after iCurrent
	inline int Add(Node<Data>* aNode) {
		aNode->iNext = iCurrent->iNext;
		iCurrent->iNext = aNode;
		iCurrent = aNode;
		return SUCCESS;
	}

	//Inserts after Nth node
	int Add(Node<Data>* aNode, int aAfter) {
		iCurrent = iHead;

		while(aAfter--) {
			iCurrent = iCurrent->iNext;
		}

		if(iCurrent) {
			return Add(aNode);
		}
		return FAILED;
	}

private:
	//Pointer to Head
	Node<Data>* iHead;
	//Last node operated
	Node<Data>* iCurrent;
};

#endif //LISTTEMPLATE_H
