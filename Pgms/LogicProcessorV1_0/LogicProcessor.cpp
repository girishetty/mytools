#include <math.h>
#include "LogicProcessor.h"
#include "LogicGates.h"

#ifndef NULL
#define NULL 0
#endif //NULL

//Definition of LogicProcessor Class

/***********************************************************************
 Factory Method for Creating Objects of Logic Family (Hierarchy)
 It takes the type of Family Type and number of inputs as input
 ***********************************************************************/
Logic* LogicProcessor::CreateLogic(LogicType aLogicType, int aNumInputs){
    Logic* pLogic = NULL;
    switch (aLogicType){
    case ENotLogic:
        {
        pLogic = new NotLogic();
        break;
        }
    case EAndLogic:
        {
        pLogic = new AndLogic(aNumInputs);
        break;
        }
    case EOrLogic:
        {
        pLogic = new OrLogic(aNumInputs);
        break;
        }
    case EXorLogic:
        {
        pLogic = new XorLogic(aNumInputs);
        break;
            }
    case EImpliesLogic:
        {
        pLogic = new ImpliesLogic();
        break;
        }
    default:
        {
        pLogic = NULL;
        break;
        }
    }
    return pLogic;
}

/***********************************************************************
 Truth Table Generating function
 It takes the type of Family Type and number of inputs as input
 ***********************************************************************/
void LogicProcessor::ShowTruthTable(LogicType aLogicType, int aNumInputs){
    Logic* pLogic = CreateLogic(aLogicType, aNumInputs);
    bool** inputTable = GenerateInputTable(aNumInputs);
    int rowSize = int(pow(2, aNumInputs));

    int i=0;
    //For each combination of input
    for (;i<rowSize;i++) {
        //Display all input in this combination
        for (int j=0; j<aNumInputs; j++) {
            DisplayValue(inputTable[i][j]);
        }
        //Process these inputs for this Logic
        bool output = pLogic->Process(inputTable[i]);
        cout<<"--> ";
        //Display output
        DisplayValue(output);
        cout<<endl;
    }

    delete pLogic;

    //Destruct the Input Table
    for (i=0;i<rowSize;i++){
        delete []inputTable[i];
    }
}

/***********************************************************************
 Processing an expression to check whether its Horizontal/Verticle 
 Symmetry Matrix
 It takes input Matrix(It should be a square Matrix) and 
 number of inputs as input and returns type of Symmetry
 ***********************************************************************/
LogicProcessor::SymmetryType LogicProcessor::IsSymmetryExpression(const char** aMatrix,
                                            int aNumInputs){
    SymmetryType result1 = EVerticle;
    SymmetryType result2 = EHorizontal;

    //Checking for Verticle Symmetry
    int startRow =0;
    int endRow = aNumInputs-1;
    int middleRow = int(aNumInputs/2)-1;
    for (; startRow < middleRow && result1 == EVerticle; startRow++, endRow--) {
        int column = 0;
        for (; column < aNumInputs; column++) {
            BoolLogic xBool(aMatrix[startRow][column]==TRUE? true: false);
            BoolLogic yBool(aMatrix[endRow][column]==TRUE? true: false);
            XorLogic xyXor(xBool, yBool);
            if (xyXor.Process()){
                result1 = ENot;
                break;
            }
        }
    }

    //Checking for Horizontal Symmetry
    int startCol =0;
    int endCol = aNumInputs-1;
    int middleCol = int(aNumInputs/2);
    for (; startCol < middleCol && result2 == EHorizontal; startCol++, endCol--) {
        int row = 0;
        for (; row < aNumInputs; row++) {
            BoolLogic xBool(aMatrix[row][startCol]==TRUE? true: false);
            BoolLogic yBool(aMatrix[row][endCol]==TRUE? true: false);
            XorLogic xyXor(xBool, yBool);
            if (xyXor.Process()){
                result2 = ENot;
                break;
            }
        }
    }

    //Categorize depending on the result
    SymmetryType result = SymmetryType(result1 | result2);
    switch (result) {
    case EHorizontal:
        cout<<"Its a Horizontal Symmetry Matrix"<<endl;
        break;
    case EVerticle:
        cout<<"Its a Verticle Symmetry Matrix"<<endl;
        break;
    case EAll:
        cout<<"Its All Symmetry Matrix"<<endl;
        break;
    case ENot:
    default:
        cout<<"Its Not a Symmetry Matrix"<<endl;
        break;
    }
    return result;
}

/***********************************************************************
 This function generates all possible combination of inputs for given 
 number of inputs. 
 It takes number of inputs as input and returns the table of 
 input combination
 ***********************************************************************/
bool** LogicProcessor::GenerateInputTable(int aNumInputs){
    int rowSize = pow(2, aNumInputs);
    bool** inputTable = NULL;
    inputTable = new bool*[rowSize];

    //Create as many columns as number of inputs
    int i = 0;
    for (;i<rowSize;i++) {
        inputTable[i] = new bool[aNumInputs];
        *inputTable[i] = '\0';
    }

    //Set the Combination of Inputs Possible
    int stepCount = rowSize;
    for (int column=0; column<aNumInputs; column++) {
        int count = 0;
        stepCount /= 2;
        do{
            //Fill with FALSE
            int step = 0;
            for (; step<stepCount; step++, count++){
                inputTable[count][column] = false;
            }
            //Fill with TRUE
            for (step = 0; step<stepCount; step++, count++){
                inputTable[count][column] = true;
            }

        }while(count != rowSize);
    }
    return inputTable;
}

/***********************************************************************
 This function display 't' or 'f' depending on truth value
 ***********************************************************************/
void LogicProcessor::DisplayValue(bool aVal){
    char dispChar = aVal? TRUE: FALSE;
    cout<<dispChar<<" ";
}
