#include <ctype.h>
#include "Node.h"
#include "NodeCollection.h"

int GetIndex(char key) {
	int index = -1;
	key = tolower(key);
	index = key - 'a';
	return index;
}

NodeCollection::NodeCollection(Node* aOwner) : iOwner(aOwner), iSize(0), iLastAccessedNode(-1) {
	for(int index=0; index<NO_OF_ALPHABETS; index++) {
		iList[index] = NULL;
	}
}

bool NodeCollection::Add(Node* pNode) {
	bool success = false;
	int index = GetIndex(pNode->Key());
	if(index != -1 && iList[index] == NULL && !iOwner->IsParentOrChild(pNode)) {
		iList[index] = pNode;
		pNode->Parent(iOwner);
		iSize++;
		success = true;
	}
	return success;
}

void NodeCollection::Remove(Node* pNode) {
	int index = GetIndex(pNode->Key());
	if(index != -1 && iList[index] == pNode) {
		iList[index] = NULL;
		pNode->Parent(NULL);
		iSize--;
	}
}

bool NodeCollection::Contains(Node* pNode) const {
	bool contains = false;
	int index = GetIndex(pNode->Key());
	if(index != -1 && iList[index] == pNode) {
		contains = true;
	}
	return contains;
}

Node* NodeCollection::operator[](int index) const {
	Node* pNode = NULL;
	if(index >=0 && index < NO_OF_ALPHABETS) {
		pNode = iList[index];
	}
	return pNode;
}

Node* NodeCollection::Find(char aKey) const {
	return iList[GetIndex(aKey)];
}

Node* NodeCollection::GetValidNode(int index) {
	Node* pNode = NULL;
	//Validate the index
	index++;
	if(index > 0 && index <= iSize) {
		for(int i = 0; i < NO_OF_ALPHABETS; i++) {
			pNode = iList[i];
			if(pNode) {
				//Got a valid node, so check the count
				index--;
				if(index == 0) {
					break;
				}
			}
		}
	}
	return pNode;
}

Node* NodeCollection::GetNextValidNode() {
	Node* pNode = NULL;
	int index = iLastAccessedNode+1;
	for(; index < NO_OF_ALPHABETS; index++) {
		pNode = iList[index];
		if(pNode) {
			//Got a valid node, so break the loop
			iLastAccessedNode = index;
			break;
		}
	}
	//There are no valid Nodes, so reset iLastAccessedNode accordingly
	if(index == NO_OF_ALPHABETS) {
		iLastAccessedNode = -1;
	}
	return pNode;
}
