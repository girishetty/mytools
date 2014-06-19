//
// FileName :             deck.h
// Compiler :             VC++ v6.0
// Author :               Virendra Shakya
// Project/Assignment :   AssignmentName
//


#ifndef __DECK_H__
#define __DECK_H__

//USER INCLUDES
#include "card.h"

//CONSTANTS
const int KDeckSize = 52;


/***************************************************************************************/
/***************************************************************************************/
// Class - CDeck
/***************************************************************************************/
/***************************************************************************************/

class CDeck
/**
* A deck is a collection of 52 cards. It is guranteed that this defintion would never change.
* Hence it makes sense to make use of array ADT for implemnting the deck abstraction.
*/
	{
public:
	//Constructors
	CDeck(); //Default constructor

	//Destructor
	~CDeck();
	

	void Shuffle();
	CCard* DrawCard(); //Draws the topmost card of the deck

	void Print(); //Used for debugging only

private:
	//Methods
	void Build(); //builds the deck - sets card-value and suit value for all 52 cards
	void SwapCards( const int aPosition1, const int aPosition2 );
	int GenerateRandomNumber() const;

private:
	//Data
	CCard mCards[ KDeckSize ];
	int mCurrentCards; //How many cards are there currently in your deck?	
	};


#endif


