#include <math.h>
#include "LogicGates.h"


char** Logic::GetIntputTable(int aInput) {
    //Create table of Rows 2 pow aInput
    int rowSize = pow(2, aInput);
    char** inputTable = NULL;
    inputTable = (char**) new char[rowSize];

    //Create as many columns as number of inputs
    int i = 0;
    for (;i<rowSize;i++) {
        inputTable[i] = new char[aInput];
        inputTable[i][aInput] = '\0';
    }

    //Set the Combination of Inputs Possible
    int stepCount = rowSize;
    for (int column=0;column<aInput;column++) {
        int count = 0;
        stepCount /= 2;
        do{
            //Fill with FALSE
            int step = 0;
            for (; step<stepCount; step++, count++){
                inputTable[count][column] = FALSE;
            }
            //Fill with TRUE
            for (step = 0; step<stepCount; step++, count++){
                inputTable[count][column] = TRUE;
            }

        }while(count != rowSize);
    }

    for (i=0;i<rowSize;i++) {
        cout<<inputTable[i]<<endl;
    }

    return inputTable;
}

void Logic::DeleteIntputTable(char** aTable, int aInput){
    int rowSize = pow(2, aInput);
    for (int i = 0;i<rowSize;i++) {
        delete [aInput] aTable[i];
    }
}

NotLogic::NotLogic(int aNumInputs ) : iNumInputs(aNumInputs), iInputTable(NULL) {
}

NotLogic::~NotLogic() {
    if (iInputTable)
        DeleteIntputTable(iInputTable, iNumInputs);
}

void NotLogic::GetTruthTable(){
    iInputTable = GetIntputTable(1);
}

const char NotLogic::GetOutput(char* aInput, int /*aCount*/){
    return(*aInput == TRUE ? FALSE : TRUE);
}

AndLogic::AndLogic(int aNumInputs ) : iNumInputs(aNumInputs), iInputTable(NULL) {
}

AndLogic::~AndLogic() {
    if (iInputTable)
        DeleteIntputTable(iInputTable, iNumInputs);
}


void AndLogic::GetTruthTable(){
    iInputTable = GetIntputTable(2);
}

const char AndLogic::GetOutput(char* aInput, int aCount){
    bool output;
    output = (aInput[0] == TRUE ? true : false);

    //Continue, only if its true
    for (int i=1; i<aCount && output; i++) {
        output &= (aInput[i] == TRUE ? true : false);
    }

    return(output == true? TRUE : FALSE );
}


OrLogic::OrLogic(int aNumInputs) : iNumInputs(aNumInputs), iInputTable(NULL) {
}

OrLogic::~OrLogic() {
    if (iInputTable)
        DeleteIntputTable(iInputTable, iNumInputs);
}

void OrLogic::GetTruthTable(){
    iInputTable = GetIntputTable(1);
}

const char OrLogic::GetOutput(char* aInput, int aCount){
    bool output;
    output = (aInput[0] == TRUE ? true : false);
    
    //Continue, only if its false
    for (int i=1; i<aCount && !output; i++) {
        output |= (aInput[i] == TRUE ? true : false);
    }

    return(output == true? TRUE : FALSE );
}
