//
// FileName :             card.cpp
// Compiler :             VC++ v6.0
// Author :               Virendra Shakya
// Project/Assignment :   AssignmentName
//

//Should be conditional hash-include for iostream.h to avoid code bloat in production code
#include <iostream.h>

#include "card.h"


//CONSTANTS
//Card Values in string - useful for printing on std-io
const char* const KCardValues[] = 
	{ 
	"Ace", 
	"Two", 
	"Three", 
	"Four", 
	"Five", 
	"Six", 
	"Seven", 
	"Eight", 
	"Nine", 
	"Ten", 
	"Jack", 
	"Queen", 
	"King" 
	};




//Suit values in string format - useful for printing
const char* const KSuitValues[] =
	{
	"Spade",
	"Diamond",
	"Heart",
	"Club"
	};



CCard::CCard() : mCardValue( eAce ), mSuit ( eSpade ), mIsWithDrawn( eFalse )
	{
	}



CCard::CCard( int aCardValue, int aSuit, bool aIsWithDrawn ) 
		: mCardValue( aCardValue ), mSuit ( aSuit ), mIsWithDrawn( aIsWithDrawn )
	{	
	}



CCard::CCard( const CCard& aRhs )
	{
	this->mCardValue = aRhs.mCardValue ;
	this->mSuit = aRhs.mSuit ;
	this->mIsWithDrawn = aRhs.mIsWithDrawn;
	}



CCard::~CCard()
	{
	}



bool CCard::operator==( const CCard& aRhs ) const
	{
	return ( 
			( 
				(this->mCardValue == aRhs.mCardValue) && 
				(this->mSuit == aRhs.mSuit)           &&
				(this->mIsWithDrawn == aRhs.mIsWithDrawn)
			) ? 
			(eTrue) : 
			(eFalse) 
		   );
	}



bool CCard::operator!=( const CCard& aRhs ) const
	{
	return (!operator==( aRhs ));
	}



//Constructions like this (a = b )= c must be avoided to avoid un-necessary instructions
const CCard& CCard::operator=( const CCard& aRhs )
	{
	this->mCardValue = aRhs.mCardValue ;
	this->mSuit = aRhs.mSuit ;
	this->mIsWithDrawn = aRhs.mIsWithDrawn;
	return *this;
	}



int CCard::CardValue() const
	{
	return mCardValue;
	}



int CCard::Suit() const
	{
	return mSuit;
	}



bool CCard::IsDrawn() const
	{
	return mIsWithDrawn;
	}



void CCard::SetCardValue( const int aCardValue ) 
	{
	//debug assert here
	mCardValue = aCardValue;
	}



void CCard::SetSuit( const int aSuit ) 
	{
	//debug assert here
	mSuit = aSuit;
	}



void CCard::SetDrawnTag( const bool aTag )
	{
	mIsWithDrawn = aTag;
	}



void CCard::Print()
	{
	cout << "CardValue = " << ShowCardValue ( mCardValue ) << "\t\t\t";
	cout << "Suit = " << ShowSuitValue( mSuit )        << "\t\t\t";
	cout << "Drawn = " << mIsWithDrawn << endl;
	}



const char* CCard::ShowCardValue( const int aCardIndex )
	{
	assert( (aCardIndex < eCardMaxSize) && (aCardIndex > 0) );

	return KCardValues[ aCardIndex - 1 ];
	}



const char* CCard::ShowSuitValue( const int aSuitIndex )
	{
	assert( (aSuitIndex < eSuitMaxSize) && (aSuitIndex > 0) );

	return KSuitValues[ aSuitIndex - 1 ];
	}
