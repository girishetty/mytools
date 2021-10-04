#include <math.h>
#include <iostream>
#include "LogicGates.h"

using std::string;
using std::cout;
using std::endl;

void Logic::CreateIntputTable() {
  //Create table of Rows 2 pow aInput
  size_t rowSize = pow(2, iNumInputs);

  string allFalse(iNumInputs, FALSE);
  for (size_t index = 0; index < rowSize; index++) {
    iInputTable.push_back(allFalse);
  }

  //Set the Combination of Inputs Possible
  size_t stepCount = rowSize;
  for (size_t column = 0; column < iNumInputs; column++) {
    size_t count = 0;
    stepCount = stepCount >> 1;
    do {
      //Fill with FALSE
      size_t step = 0;
      for (; step < stepCount; step++){
        iInputTable[count++][column] = FALSE;
      }
      //Fill with TRUE
      for (step = 0; step<stepCount; step++){
        iInputTable[count++][column] = TRUE;
      }

    } while(count != rowSize);
  }
}

void Logic::PrintTruthTable() const {
  for (auto row : iInputTable) {
    cout << "|" << row << "| ==> [" << GetOutput(row) << "]" << endl;
  }
}

char NotLogic::GetOutput(const string& aInput) const {
  return (aInput[0] == TRUE ? FALSE : TRUE);
}

char AndLogic::GetOutput(const string& aInput) const {
  bool output = (aInput[0] == TRUE ? true : false);

  //Continue, only if its true
  for (size_t i = 1; i < aInput.size() && output; i++) {
    output &= (aInput[i] == TRUE ? true : false);
  }

  return(output == true? TRUE : FALSE );
}


char OrLogic::GetOutput(const string& aInput) const {
  bool output = (aInput[0] == TRUE ? true : false);
  
  //Continue, only if its false
  for (size_t i = 1; i < aInput.size() && !output; i++) {
    output |= (aInput[i] == TRUE ? true : false);
  }

  return(output == true? TRUE : FALSE );
}
