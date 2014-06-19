//
// FileName :             card.h
// Compiler :             VC++ v6.0
// Author :               Virendra Shakya
// Project/Assignment :   AssignmentName
//


#ifndef __CARD_H__
#define __CARD_H__

//USER INCLUDES
#include "common.h"

/***************************************************************************************/
/***************************************************************************************/
// Class - CCard
/***************************************************************************************/
/***************************************************************************************/

class CCard
/**
* Main abstraction for a card. Stores the suit and card face value.
*/
	{
public:
	//Constructors
	CCard();//Default constructor
	CCard( int aCardValue, int aSuit, bool aIsWithDrawn = eFalse );
	CCard( const CCard& aRhs ); //Copy constructor

	//Destructor
	~CCard();

	//SETTERS & ACCESSORS
	int CardValue() const;
	int Suit() const;
	bool IsDrawn() const;

	void SetCardValue( const int aCardValue ) ;
	void SetSuit( const int aSuit ) ;
	void SetDrawnTag( const bool aTag = eFalse );

	bool operator==( const CCard& aRhs ) const;
	bool operator!=( const CCard& aRhs ) const;
	const CCard& operator=( const CCard& aRhs );

	void Print();//Used for debugging

private:
	//methods
	const char* ShowCardValue( const int aCardIndex );
	const char* ShowSuitValue( const int aSuitIndex );

private:
	//Data
	int mCardValue; 
	int mSuit; 
	bool mIsWithDrawn;
	};


#endif


