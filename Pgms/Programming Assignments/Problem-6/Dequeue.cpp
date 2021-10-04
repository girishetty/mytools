
/*
 * Problem:
 * Implement a deque.  Think about what properties a deque must have.
 * Implement it as a base class that can be extended and as a template.
 * In general, start thinking about what's underneath all the other STL
 * data types and how they work.
 * Solution:
 * Define a dequeue which operates on "type" less data type, then define a thin template for specific data type 
*/

class Node {
private:
	void*   pElement;
	DeList* pNext;
	DeList* pPrev;

	friend void InsertAfter(Node* pNode, void* element);
	friend void InsertBefore(Node* pNode, void* element);
	friend void* Remove(Node* pNode);
	friend class BaseDequeue;
}

//Inserts an element afetr a specified node
//if first argument is NULL, then its the first element
//Returns the new node
Node* InsertAfter(Node* pNode, void* element);
//Inserts an element before a specified node
//if first argument is NULL, then its the first element
//Returns the new node
Node* InsertBefore(Node* pNode, void* element);
//Removes and return an element pointed by Node
void* Remove(Node* pNode);

//Define the base class with all the functionalitie defined here
class BaseDequeue {
public:
	BaseDequeue() : pFront(NULL), pTail(NULL), size(0){
	}

	BaseDequeue(const BaseDequeue& dq) : pFront(NULL), pTail(NULL), size(dq.size){
		//construct the Dequeue from the existing one
		CopyFrom(dq, size);
	}
	
	void PushBack(void* element) {
		pTail = InsertAfter(pTail, element);
		size++;
	}

	void PushFront(void* element) {
		pFront = InsertBefore(pFront, element);
		size++;
	}

	void* PopFront() {
		Node* popNode = pFront;
		pFront = pFront->pNext;
		size--;
		return Remove(popNode->pElement);
	}

	void* PopBack() {
		Node* popNode = pTail;
		pTail = pTail->pPrev;
		size--;
		return Remove(popNode->pElement);
	}

	void* Front() {
		return pFront->pElement;
	}

	void* Tail() {
		return pTail->pElement;
	}

	//Get nth element in the deque from Front
	void* GetAt(UINT index) const {
		void* pElement = NULL;
		if(index<size) {
			Node* pIndex = pFront;
			//Loop thru the deque
			for(; index > 0; index--, pIndex = pIndex->pNext);
			pElement = pNode->pElemen);
		}
		return pElement;
	}

	inline UINT Size() const {
		return size;
	}

	inline bool IsEmpty() const {
		return size==0? true : false;
	}

protected:
	//Copy from a source deque to this one.
	void CopyFrom(const BaseDequeue& dq, UINT total){
		Node* pIndex = dq.pFront;
		if(pFront){
			pFront = pTail = InsertAfter(NULL, pIndex->PElement);
		}
		//Loop thru the deque
		for(pIndex = pIndex->pNext; total > 0; total--, pIndex = pIndex->pNext){
			pTail = InsertAfter(pTail, pIndex->PElement);
		}
	}

	//return the element without modifying the state
	inline void* GetElement(Node* pNode) const {
		return pNode->PElement;
	}

private:
	Node* pFront;
	Node* pTail;
	UINT  size;
};


//Define a thin template to avoid all the problems with template
template<class Type> class Dequeue : public BaseDequeue {
public:
	Dequeue() {	}

	Dequeue(const Dequeue& dq) : BaseDequeue(dq) { }
	}
	
	void PushBack(Type* element) {
		BaseDequeue::PushBack(element);
	}

	void PushFront(Type* element) {
		BaseDequeue::PushBack(pFront, element);
	}

	Type* PopFront() {
		return (Type*) BaseDequeue::PopFront();
	}

	Type* PopBack() {
		return (Type*) BaseDequeue::PopBack();
	}

	Type* Front() {
		return (Type*) BaseDequeue::Front();
	}

	Type* Tail() {
		return (Type*) BaseDequeue::Tail();
	}

	//Get nth element in the deque from Front
	Type* operator[](UINT index) const {
		return GetAt(index);
	}

	//Get nth element in the deque from Front
	Type* GetAt(UINT index) const {
		return (Type*) BaseDequeue::GetAt(index);
	}
};
