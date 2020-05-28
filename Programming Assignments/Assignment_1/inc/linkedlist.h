//
// FileName :             linkedlist.h
// Compiler :             VC++ v6.0
// Author :               Virendra Shakya
// Project/Assignment :   AssignmentName
//



#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__



//USER INCLUDES
template <class ADT> class CLinkedList;
template <class ADT> class CLLIterator;



//SYSTEM INCLUDES
#include <stdlib.h>



//FORWARD DECLARAIONS
class CLLIteratorBase;



/***************************************************************************************/
/***************************************************************************************/
// Class - CLinkedListNode
/***************************************************************************************/
/***************************************************************************************/

template <class ADT>
class CLinkedListNode
/**
* A generic linked list node defintion
*/
	{
	friend class CLinkedList<ADT>;
	friend class CLLIterator<ADT>;
public:
	//Constructors
	CLinkedListNode();
	CLinkedListNode( ADT aData ) : mInfo( aData ), mNext( NULL ) {}
	~CLinkedListNode(){ mNext = NULL; }
	void Print();

private:
	//Data
	ADT mInfo;
	CLinkedListNode<ADT> *mNext;
	
	};

template<class ADT>
CLinkedListNode<ADT>::CLinkedListNode()
	{
	mNext = NULL;
	}

template <class ADT>
void CLinkedListNode<ADT>::Print()
	{
	mInfo.Print();
	}



/***************************************************************************************/
/***************************************************************************************/
// Class - CLinkedList
/***************************************************************************************/
/***************************************************************************************/

//CLASS DECLARATION
template <class ADT>
class CLinkedList
/**
* A generic linked list. 
*/
	{
public:
	CLinkedList();
	~CLinkedList();
	int AddNode( const ADT& aData );
	void PrintList();
	int Size() const;
	void DeleteAll();
	void DeleteHead();
	ADT* GetNode();

private:
	bool IsEmpty() const;

private:
	CLinkedListNode<ADT> *mHead;
	};




template <class ADT>
//CLinkedList<ADT>::CLinkedList() : mHead( NULL ), mCurr(NULL)
CLinkedList<ADT>::CLinkedList() : mHead( NULL )
	{	
	}



template <class ADT>
CLinkedList<ADT>::~CLinkedList()
	{
	DeleteAll();
	}



template <class ADT>
bool CLinkedList<ADT>::IsEmpty() const
	{
	return ( NULL == mHead ) ? eTrue : eFalse;
	}



template <class ADT>
int CLinkedList<ADT>::AddNode( const ADT& aData )
	{
	int errCode = eSuccess;

	CLinkedListNode<ADT> *node = NULL;
	node = new CLinkedListNode<ADT>( aData );
	if( NULL == node )
		{
		return eOutOfMemory;
		}

	if( eTrue == IsEmpty() )
		{
		mHead = node;
		return eSuccess;
		}

	//traverse till end of the list
	CLinkedListNode<ADT> *curr = NULL;
	curr = mHead;
	while( NULL != curr->mNext )
		{
		curr = curr->mNext;
		}

	//Out of while loop...add the node at the end now
	curr->mNext = node;
	return errCode;
	}



template <class ADT>
void CLinkedList<ADT>::PrintList()
	{
	
	CLinkedListNode<ADT> *curr = NULL;
	curr = mHead;

	while( NULL != curr->mNext )
		{
		curr->Print();
		curr = curr->mNext;		
		}

	//After coming out of while loop, you are still pointing at the last node
	//of the list - print it !
	curr->Print();
	}



template <class ADT>
int CLinkedList<ADT>::Size() const
	{
	int size = 0;
	CLinkedListNode<ADT> *curr = NULL;
	curr = mHead;

	while( NULL != curr->mNext )
		{
		size++;
		curr = curr->mNext;		
		}

	//After coming out of while loop, you are still pointing at the last node
	//of the list - print it !
	size++;
	return size;
	}



template <class ADT>
void CLinkedList<ADT>::DeleteHead()
	{
	CLinkedListNode<ADT> *curr = NULL;
	curr = mHead->mNext;
/*
	if( NULL != mHead )
		curr = mHead->mNext;
	else
		return;
*/
	delete mHead;
	mHead = NULL;

	mHead = curr;
	}



template <class ADT>
void CLinkedList<ADT>::DeleteAll()
	{
	CLinkedListNode<ADT> *curr = NULL;
	curr = mHead;

	while( NULL != curr->mNext )
		{
		curr = curr->mNext;
		DeleteHead();		
		}

	DeleteHead();

	}



template <class ADT>
ADT* CLinkedList<ADT>::GetNode()
{
	if( NULL != mHead )
		return &mHead->mInfo;

	return NULL;
}


/***************************************************************************************/
/***************************************************************************************/
// Class - CLLIterator
/***************************************************************************************/
/***************************************************************************************/

template <class ADT>
class CLLIterator
/**
* Provides a way to leaf thru the nodes of the linked list. This is a forward iterator only 
* as of now.
*/
	{
public:
	CLLIterator() : mLinkedList(NULL) {}
	CLLIterator( CLinkedList<ADT> *aLinkedList );

	void Next();
	ADT* Current();

private:
	CLinkedList<ADT> *mLinkedList;
	CLinkedListNode<ADT> *mCurrentNode;
	};



template <class ADT>
CLLIterator<ADT>::CLLIterator( CLinkedList<ADT> *aLinkedList )
	{
	mLinkedList = aLinkedList;
	}



template <class ADT>
void CLLIterator<ADT>::Next()
	{
	if( NULL != mCurrentNode )
		{
		mCurrentNode = mCurrentNode->mNext ;
		}
	}



template <class ADT>
ADT* CLLIterator<ADT>::Current()
	{
	return &mCurrentNode->mInfo;
	}

#endif