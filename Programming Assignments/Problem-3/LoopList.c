/* Problem:
 * Given the root node to a singly linked list, write an algorithm to
 * see if it loops back on itself somewhere in the middle.
 * Solution:
 * use 2 pointers to traverse thru the list, with one of them goes 2 nodes each time. 
 * The second one goes at 1 nodes each time.
 * If there is a cycle, the one that goes 2 nodes each time will eventually meet the one that goes slower.
 * If that is the case, then you will know the linked-list is a cycle.
 */

typedef unsigned int UINT;

typedef struct Node {
	int   data;
	struct Node* next;
}Node;


bool IsLoopList(Node* pHead) {
	Node* p1 = pHead;
	Node* p2 = pHead;
	bool loop = true;

	if(pHead != NULL) {
		do{
			p1 = p1->next;
			p2 = p2->next;
			if(p2 != NULL) {
				p2 = p2->next;
			}
			if(p1 == NULL || p2 == NULL)
			{
				loop = false;
				break;
			}

		}while ( p1 != p2 );
	}

	return loop;
}
