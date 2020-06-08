#include <iostream>
#include "Deck.h"

using namespace std;

int main() {
	Deck deck;
	deck.Display();
	deck.Shuffle();
	deck.Display();

	return 0;
}

