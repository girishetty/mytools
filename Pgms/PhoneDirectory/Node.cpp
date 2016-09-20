#include <iostream>
using namespace std;
#include <string.h>
#include "Entry.h"
#include "Node.h"
#include "NodeCollection.h"

Node::Node(char aKey, const Entry& aEntry) : iParent(NULL), iKey(aKey), iIsTerminal(false) {
	iEntry = new Entry(aEntry);
    //creates an empty collection
    iChildren = new NodeCollection(this);
}

Node::~Node() {
	delete iEntry;
    delete iChildren;
}

//Gets the root node
const Node* Node::Root() const {
	if(iParent) {
		return iParent->Root();
	}
	return this;
}

//Determins if this node is ancestor of the given node
bool Node::IsParentOfNode(Node* pNode) const {
	bool isParent = false;
	if (iChildren->Contains(pNode)) {
		isParent = true;
	}
    if(!isParent) {
	    int childCount = iChildren->Count();
	    Node* pChild = NULL;
	    for(int index = 0; index < childCount; index++) {
		    pChild = iChildren->GetValidNode(index);
		    if (pChild && pChild->IsParentOfNode(pNode)) {
			    isParent = true;
			    break;
		    }
	    }
    }
    return isParent;
}

//Determins if this node is descendant of the given node hierarchy
bool Node::IsChildOfNode(Node* pNode) const {
	bool success = false;

	if (!iParent) {
		success  = false;
	}
	else if (pNode == iParent) {
		success = true;
	}
	else {
		success = iParent->IsChildOfNode(pNode);
	}
	return success;
}

//determines if this node shares hierarchy with given node
bool Node::IsParentOrChild(Node* pNode) const {
	bool success = false;
	if (pNode == this) {
		success = true;
	}
	else if (IsParentOfNode(pNode)) {
		success = true;
	}
	else if (IsChildOfNode(pNode)) {
		success = true;
	}
    return success;
}

//Get the corresponding Child Node
Node* Node::GetChildNodeByKey(char aKey) const{
	Node* childNode = NULL;
	if(aKey != '\0') {
		childNode = iChildren->Find(aKey);
	}
	return childNode;
}

bool Node::AddChild(Node* pChild) {
	return iChildren->Add(pChild);
}

void Node::DisplayDepthNodes() const {
	//If its a terminal Node, display the value
	if(IsTerminal()) {
		iEntry->Display();
	}
	
	//Now do DFS on each child
	int childCount = iChildren->Count();
	iChildren->ResetLastValidNode();
	Node* pChild = NULL;
	for(int index = 0; index < childCount; index++) {
		pChild = iChildren->GetNextValidNode();
		if(pChild) {
			pChild->DisplayDepthNodes();
		}
	}
}

void Node::DeleteDepthNodes() {
	//Now do DFS on each node and delete the Nodes from that order
	int childCount = iChildren->Count();
	iChildren->ResetLastValidNode();
	for(int index = 0; index < childCount; index++) {
		Node* pChild = iChildren->GetNextValidNode();
		if(pChild) {
			pChild->DeleteDepthNodes();
		}
	}
#if _DEBUG
	if(IsTerminal()) {
		iEntry->Display();
	}
	else{
		cout<<"NTNode with key: "<<iKey<<endl;
	}
#endif
	delete this;
}
