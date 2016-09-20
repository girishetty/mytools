#include <iostream>
#include "LinkedList.h"
using namespace std;

/*
 * Node class implementation
 */
//Constructors
Node::Node(const string& aData, Node* pNext) : iData(aData), iNext(pNext)
{
}
Node::Node(const char* pData, Node* pNext) : iData(pData), iNext(pNext)
{
}
Node::Node(const Node* pNode) : iData(pNode->iData), iNext(pNode->iNext)
{
}
//Copy Constructor
Node::Node(const Node& aNode) : iData(aNode.iData), iNext(aNode.iNext)
{
}

//Destructor
Node::~Node()
{
}

const string& Node::Value() const
{
    return iData;
}

//Assignment operator
Node& Node::operator=(const Node& aNode) {
    if (this != &aNode) {
        iData = aNode.iData;
        iNext = aNode.iNext;
    }
    return *this;
}

/*
 * Linked List class implementation
 */
//Public Constructor
LinkedList::LinkedList() : iHead(NULL), iLength(0)
{
}

//Copy Constructor
LinkedList::LinkedList(const LinkedList& aList)
{
}

//Assignment operator
LinkedList& LinkedList::operator=(const LinkedList& aList)
{
    return *this;
}

//Destructor that deletes individual nodes from the List
LinkedList::~LinkedList()
{
    Node* pNode = iHead;

    while (pNode) {
        iHead = iHead->iNext;
        delete pNode;
        pNode = iHead;
    }
}

//Insert a Node to end of this List
bool LinkedList::Insert(Node* pNode, bool bOwnIt)
{
    bool  bSuccess = false;
    Node* item = NULL;
    Node* p = NULL;

    if (bOwnIt) {
        //Ownership is transferred
        item = pNode;
    } else if (pNode) {
        //Make own copy
        item = new Node(pNode);
    }
    if (item) {
        if (!iHead) {
            //First item, set the head
            iHead = item;
        } else {
            //Insert in the end
            p = iHead;
            while (p->iNext) {
                p = p->iNext;
            }
            p->iNext = item;
        }
        iLength++;
        bSuccess = true;
    }

    return bSuccess;
}

bool LinkedList::Insert(const string& aData)
{
    Node* pNode = new Node(aData);
    return Insert(pNode);
}

bool LinkedList::Insert(const char* pData)
{
    Node* pNode = new Node(pData);
    return Insert(pNode);
}

//Locate a Node by its value, and also returns the previous Node
Node* LinkedList::Locate(const string& aData, Node** pPrevNode) const
{
    bool  bFound = false;
    Node* pNode = iHead;
    Node* pLastNode = iHead;

    while (pNode) {
        if (pNode->iData.compare(aData) == 0) {
            bFound = true;
            break;
        }
        pLastNode = pNode;
        pNode = pNode->iNext;
    }

    if (!bFound) {
        pNode = NULL;
    } else if (pPrevNode) {
        *pPrevNode = pLastNode;
    }
    return pNode;
}

//Remove a Node by its value
bool LinkedList::Remove(const string& aData)
{
    bool  bFound = false;
    Node* pNode = NULL;
    Node* pPrevNode = NULL;

    pNode = Locate(aData, &pPrevNode);
    if (pNode) {
        if (pNode == iHead) {
            //Move the Head now
            iHead = iHead->iNext;
        } else {
            //Rearrange the link
            pPrevNode->iNext = pNode->iNext;
        }
        delete pNode;
        bFound = true;
        iLength--;
    }

    return bFound;
}

bool LinkedList::Remove(const char* pData)
{
    bool   bFound = false;
    string data(pData);

    if (pData) {
        bFound = Remove(data);
    }

    return bFound;
}

//Find a Node by its value
Node* LinkedList::Find(const string& aData) const
{
    return Locate(aData, NULL);
}

Node* LinkedList::Find(const char* pData) const
{
    Node*  pNode = NULL;
    string data(pData);

    if (pData) {
        pNode = Locate(data, NULL);
    }

    return pNode;
}

size_t LinkedList::Size() const
{
    return iLength;
}

void LinkedList::Print() const
{
    Node* pNode = iHead;

    cout << "List (" << iLength << ") = [ ";
    while (pNode) {
        cout << "\"" << pNode->iData << "\" ";
        pNode = pNode->iNext;
    }

    cout << "]" << endl;
}
