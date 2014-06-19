//
// FileName :             hand.cpp
// Compiler :             VC++ v6.0
// Author :               Virendra Shakya
// Project/Assignment :   AssignmentName
//


#include "hand.h"
#include "card.h"
#include "common.h"



CHand::CHand()
	{	
	mNumOfCards = mList.Size(); //How many hands are currently present?
	}



CHand::~CHand()
	{
	mList.DeleteAll();
	}



void CHand::RemoveCard()
	{
	mList.DeleteHead();
	mNumOfCards--;
	}



//Adding cards to a hand
int CHand::AddCard( const CCard& aCard )
	{
	mNumOfCards++;
	return mList.AddNode( aCard );
	}



void CHand::Next()  
	{	
	mListIterator.Next();
	}



CCard* CHand::InspectCard()
	{
	return mListIterator.Current()->GetNode();
	}
