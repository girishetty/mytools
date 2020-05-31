#include <iostream>
using namespace std;
#include "MagicSquare.h"

int main() {
  int num = 0;
  cout<<"Enter an odd no (preferably less than 10) : ";
  cin>>num;
  //if even num, make it odd
  if(!(num & 0x01)) {
    num++;
  }

  MagicSquare m(num);
  m.Create();
  m.Display();

  vector<vector<int>> square = { {4, 8, 2},
                                 {4, 5, 7},
                                 {6, 1, 6}};

  cout << "Cost is: " << m.fixMagicSquare(square) << endl;
  return 0;
}
