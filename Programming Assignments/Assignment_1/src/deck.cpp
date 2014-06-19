//
// FileName :             deck.cpp
// Compiler :             VC++ v6.0
// Author :               Virendra Shakya
// Project/Assignment :   AssignmentName
//

#include <stdlib.h> //Needed for srand() function
//Should be conditional hash-include for iostream.h to avoid code bloat in production code
#include <iostream.h>

#include "deck.h"
#include "common.h"



CDeck::CDeck()
	{
	Build();
	}



CDeck::~CDeck()
	{
	}



//Assigns a particular card-value and suit value to every card of this deck
void CDeck::Build()
	{
	int cardNum = 0;
	for( int suits = 1; suits < eSuitMaxSize; suits++ )
		{
		for( int cardVal = 1; cardVal < eCardMaxSize; cardVal++ )
			{
			mCards[ cardNum ].SetCardValue( cardVal );
			mCards[ cardNum ].SetSuit( suits );
			cardNum++;//Increment the card number
			}
		}
	}



//Shuffling is always done on a deck of card.
void CDeck::Shuffle()
	{
	for(int i = 0; i < KDeckSize; i++ )
		{
		int randomNumber = GenerateRandomNumber();
		
		//Assert that the random number generated is indeed less than 52
		assert( randomNumber < KDeckSize );

		SwapCards( i, randomNumber );
		}
	}



void CDeck::SwapCards( const int aPosition1, const int aPosition2 )
	{
	assert( (aPosition1 >= 0) && (aPosition2 < KDeckSize) );

	CCard temp;
	temp = mCards[ aPosition1 ];
	mCards[ aPosition1 ] = mCards[ aPosition2 ];
	mCards[ aPosition2 ] = temp;
	}



//generates random number between 0 to 51
int CDeck::GenerateRandomNumber() const
	{
	int randomNum = 0;
	randomNum = rand() % KDeckSize ;	
	return randomNum;
	}


//Randomly removes/draws a card from the deck
//Returns NULL pointer if Out-Of-Memory is encountered
//Note that it is the responisbility of the user of this class
//to delete this card by invoking delete.
CCard* CDeck::DrawCard()
	{
	CCard *card = NULL;
	int randomNum = GenerateRandomNumber();
	assert( randomNum < KDeckSize );

	//Create a copy of this card and return to clients
	card = new CCard( mCards[ randomNum ].CardValue(), mCards[ randomNum ].Suit () );	

	//This card should be marked as "withdrawn"
	mCards[ randomNum ].SetDrawnTag( eTrue );//Yes, its not in deck now

	//You drew one card, so update the mCurrentCards member variable
	mCurrentCards--;
	return card;
	}



void CDeck::Print()
	{
	for( int i = 0; i < KDeckSize; i++ )
		{
		mCards[ i ].Print();
		}
	}