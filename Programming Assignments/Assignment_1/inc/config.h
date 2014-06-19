//
// FileName :             config.h
// Compiler :             VC++ v6.0
// Author :               Virendra Shakya
// Project/Assignment :   AssignmentName
//

//This file contains some enum types that may be useful for the users of card/deck/hand classes

#ifndef __CONFIG_H__
#define __CONFIG_H__

enum EBoolValues { eFalse, eTrue };

enum ECardValue
	{
	eAce = 1,
	eTwo,
	eThree,
	eFour,
	eFive,
	eSix,
	eSeven,
	eEight,
	eNine,
	eTen,
	eJack,
	eQueen,
	eKing,
	eCardMaxSize
	};

enum ESuit
	{
	eSpade = 1,
	eDiamond,
	eHeart,
	eClub,
	eSuitMaxSize
	};



#endif

