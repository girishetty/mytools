#include <iostream>
#include <stdlib.h>
#include "Deck.h"
using namespace std;

void Cards::Set(Suit aSuit, int aVal)
{
    iSuit = aSuit;
    iVal = aVal;
}

void Cards::Display() const
{
    const char* str = NULL;
    switch(iSuit) {
    case CLUBS:
        str = "CLUB ";
        break;
    case SPADES:
        str = "SPADE ";
        break;
    case HEARTS:
        str = "HEART ";
        break;
    case DIAMONDS:
        str = "DIAMOND ";
        break;
    }
    cout<<str<<iVal<<endl;
}

Deck::Deck()
{
    int index = 0;
    for (int suit=CLUBS; suit<=DIAMONDS; suit++) {
        for (int val=1; val<=kCardSetCount; val++) {
            iCards[index++].Set((Suit)suit, val); 
        }
    }
}

void Deck::Display() const {
    for (int index=0; index<kCardDeckCount; index++) {
        iCards[index].Display();
    }
}

void Deck::Shuffle() {
    int j = 0;
    Cards temp;
    for (int index = (kCardDeckCount - 1); index>1; index--) {
        j = rand() % (index+1);
        temp = iCards[index];
        iCards[index] = iCards[j];
        iCards[j] = temp;
    }
}
