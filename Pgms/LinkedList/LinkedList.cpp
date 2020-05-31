# include <iostream>
using namespace std;

const int SUCCESS = 0;
const int FAILED = -1;

struct Node {
	inline Node(int aData = 0, Node* aLink = NULL) : iData(aData), iNext(aLink) {
	}

	inline void Display() {
		cout<<iData<<" ";
	}

	int iData;
	Node* iNext;
};

class List {
public:
	inline List() : iHead(NULL), iCurrent(NULL) {
		iHead = iCurrent = new Node;
	}

	~List() {
		while(iHead){
			iCurrent = iHead->iNext;
			delete iHead;
			iHead = iCurrent;
		}
	}

	void Display() {
		Node* p = iHead->iNext;
		cout<<"List Starts : ";
		while(p){
			p->Display();
			p = p->iNext;
		}
		cout<<"List Ends"<<endl;
	}

	//Inserts after iCurrent
	inline int Add(Node* aNode) {
		aNode->iNext = iCurrent->iNext;
		iCurrent->iNext = aNode;
		iCurrent = aNode;
		return SUCCESS;
	}

	//Inserts after Nth node
	int Add(Node* aNode, int aAfter) {
		iCurrent = iHead;

		while(aAfter--) {
			iCurrent = iCurrent->iNext;
		}

		if(iCurrent) {
			return Add(aNode);
		}
		return FAILED;
	}

	inline Node* NextNextInList(Node* aNode) {
		Node* nextInList = aNode;
		if(nextInList){
			nextInList = nextInList->iNext;
			if(nextInList && nextInList->iNext){
				nextInList = nextInList->iNext;
			}
		}
		return nextInList;
	}

	//Before 1->2->3->4->5->6->7->8->NULL
	//After  1->3->2->5->4->7->6->8->NULL
	void ReorderList() {
		Node* traverse = iHead->iNext;
		Node* nextInList = traverse->iNext;
		Node* nextNextInList = NextNextInList(traverse);
		Node* temp;

		while(traverse) {
			traverse->iNext = nextNextInList;
			traverse = nextInList;

			if(nextNextInList) {
				nextInList = nextNextInList->iNext;
			}
			else {
				nextInList = NULL;
			}

			temp = NextNextInList(nextNextInList);
			if(nextNextInList) {
				nextNextInList->iNext = traverse;
			}
			nextNextInList = temp;
		}
	}

private:
	//Pointer to Head
	Node* iHead;
	//Last node operated
	Node* iCurrent;
};



int main() {
	const int MaxNode = 10;
	List* myList = new List;
	Node* nodes[MaxNode];

	for(int i=0;i<MaxNode; i++) {
		nodes[i] = new Node(i+1);
		myList->Add(nodes[i]);
	}


	myList->Display();
	myList->ReorderList();
	myList->Display();

	return 0;
}
