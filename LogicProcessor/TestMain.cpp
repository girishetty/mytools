#include <iostream>
#include "LogicGates.h"

using std::string;
using std::cout;
using std::endl;

int main() {
  NotLogic* pNot = new NotLogic;
  string notInput("t");
  cout << "NOT of " << notInput << " is " << pNot->GetOutput(notInput) << endl;
  pNot->PrintTruthTable();
  delete pNot;

  OrLogic* pOr = new OrLogic;
  string orInput = "tfffft";
  cout << "OR of " << orInput << " is " << pOr->GetOutput(orInput) << endl;
  pOr->PrintTruthTable();
  delete pOr;

  AndLogic* pAnd = new AndLogic(4);
  string andInput = "tfffft";
  cout << "AND of " << andInput << " is " << pAnd->GetOutput(andInput) << endl;
  pAnd->PrintTruthTable();
  delete pAnd;

  return 0;
}
