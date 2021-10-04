//
// FileName :             blackjack.h
// Compiler :             VC++ v6.0
// Author :               Virendra Shakya
// Project/Assignment :   AssignmentName
//

// This file contains the interface defintions for the blackjack assignment.
// None of the classes have been implemented.

/***************************************************************************************/
/***************************************************************************************/
// Class - CGame
/***************************************************************************************/
/***************************************************************************************/

class CGame
/**
* This is an abstract base class that puts forward certain contracts for all the games.
*/
	{
public:
	//Destructors
	virtual ~CGame();

	//Methods
	virtual void Start() = 0; //Start the game
	virtual void Stop() = 0; //Stop the game
	virtual void Pause() = 0; //Pause for some time
	virtual void Resume() = 0; //Reseume playing the game
	};




/***************************************************************************************/
/***************************************************************************************/
// Class - CBlackJack
/***************************************************************************************/
/***************************************************************************************/
const int KCardSets = 3;

class CBlackJack : public CGame
	{
public:
	CBlackJack();
	~CBlackJack();

	void Start();
	void Stop();
	void Pause();
	void Resume(); 

private:
	CBlackJackHand *iHand;
	CCard *iCardSets[ KCardSets ];
	};



/***************************************************************************************/
/***************************************************************************************/
// Class - CBlackJackHand
/***************************************************************************************/
/***************************************************************************************/

class CBlackJackHand : public CHand
/**
* A hand for BlackJack game
*/
	{
public:
	CBlackJackHand( int aDecNum, CCard& aCard1, CCard& aCard2 );
	~CBlackJackHand();

	bool IsBusted() const;

	//Dont draw more cards. Must be last action
	void StandAction() const;
	void HitAction();

	//Draw one additional card only
	void DoubleDown();

	CHand* Split( CCards& aCardPair);
	CCard* ReceiveAdditonalCard();

private:
	//Methods
	bool IsLastAction() const;
	bool WasInitialValue_9_10_11() const;
	bool HadSameInitalValue(CCards& aCards ) const ;
		
	};
