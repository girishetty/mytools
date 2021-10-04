#ifndef _DECK_H
#define _DECK_H

#include <iostream>
#include <stdlib.h>
using namespace std;

const unsigned int kCardSetCount = 13;
const unsigned int kCardDeckCount = 4 * kCardSetCount;

enum Suit {
    CLUBS = 0,
    SPADES,
    HEARTS,
    DIAMONDS
};

class Cards {
public:
    Cards() {}
    Cards(Suit aSuit, int aVal) : iSuit(aSuit), iVal(aVal) {}
    void Set(Suit aSuit, int aVal);
    void Display() const;

private:
    Suit iSuit;
    int  iVal;
};

class Deck {
public:
    Deck();
    void Display() const;
    void Shuffle();

private:
    Cards iCards[kCardDeckCount];
};

#endif
