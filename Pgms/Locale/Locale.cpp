#include <string>
#include <vector>
#include <iostream>
#include <locale>
//#include <codecvte.h>

int main ()
	{
	using namespace std;

	locale loc;	// Default locale

	locale my_loc(loc); 
	// imbue modified locale onto cout
	locale old =	cout.imbue(my_loc);
	cout	<< "A jolly time was had by all" << endl;

	cout.imbue(old);
	cout	<< "A jolly time was had by all" << endl;

	cout	<< endl;
	return 0;
	}
