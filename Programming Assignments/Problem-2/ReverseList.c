/* Problem:
 * Given the root node to a singly linked list, reverse the last 5(n) nodes in the list.
 * Solution:
 * Giving a generic solution by implementing few algorithms to reverse last n nodes in the list
 */

typedef unsigned int UINT;

typedef struct Node {
	int   data;
	struct Node* next;
}Node;

Node* GetNthNodeFromLast(Node* pHead, UINT n);
Node* ReverseList(Node* pHead);

void ReverseLastNNodesInList(Node* pHead, UINT n) {
	/* validate the list first */
	if(pHead != NULL) {
		/*Get n+1th Node from last in this list */
		Node* pNthLastNode = GetNthNodeFromLast(pHead, n+1);
		/* reverse the list from nth node */
		Node* pReversedLastNList = ReverseList(pNthLastNode->next);
		/* make n+1th node point to the reversed list */
		pNthLastNode->next = pReversedLastNList;
	}
}

/* assumes that pHead is valid */
Node* GetNthNodeFromLast(Node* pHead, UINT n){
	Node* p1 = pHead;
	Node* p2 = pHead;

	/* Now, traverse through the list N times so that p1 points to nth NODE */
	for(;n!=0 && p1 != NULL; n--, p1->next);

	/* p1 is N nodes away from p2(head) now
	 * traverse through both p1 and p2, till p1 points to NULL (end of the node) 
	 */
	for(;p1!=NULL; p2=p2->next, p1=p1->next);

	/* Now, p2 points to Nth Node from the last */
	return p2;
}

/* assumes that pHead is valid */
Node* ReverseList(Node* pHead) {
	Node* pNextNextNode = NULL;
	Node* pNode = pHead;
	Node* pNextNode = pNode->next;
	pNode->next = NULL;
	/* traverse thru the list and reverse it */
	while(pNextNode != NULL) {
		pNextNextNode = pNextNode->next;
		pNextNode->next = pNode;
		pNextNode = pNextNextNode;
		pNode = pNextNode;
	}
	/* pNode is head of this list now */
	return pNode;
}
