//
// FileName :             hand.h
// Compiler :             VC++ v6.0
// Author :               Virendra Shakya
// Project/Assignment :   AssignmentName
//


#ifndef __HAND_H__
#define __HAND_H__

//USER INCLUDES
#include "linkedlist.h"

//FORWARD DECLARAIONS
class CCard;

/***************************************************************************************/
/***************************************************************************************/
// Class - CHand
/***************************************************************************************/
/***************************************************************************************/

class CHand
/**
* Description of the class in one or two lines
*/
	{
public:
	CHand();//Default constructor

	//Destructor
	~CHand();

	int AddCard( const CCard& aCard );
	void RemoveCard();
	CCard* InspectCard();

	void Next() ;//Go to the next card. Typically, the user should invoke Next() followed by InspectCard() call to see the card

private:
	//Data
	CLinkedList<CCard> mList;
	int mNumOfCards; //Number of cards in this hand

	CLLIterator< CLinkedList< CCard > > mListIterator; //Used for browing thru' the cards contained in a hand.
	};




#endif


