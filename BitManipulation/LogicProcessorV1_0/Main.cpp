#include "LogicProcessor.h"
#include "LogicGates.h"
#include "TestCases.h"
#include <iostream>
using namespace std;

/**************************************************************************
 Test for displaying TRUTH TABLE
 **************************************************************************/
void TruthTableGenTest(){

    LogicProcessor* pLogicProcessor = new LogicProcessor;

    cout<<"Testing NOT Logic"<<endl;
    pLogicProcessor->ShowTruthTable(LogicProcessor::ENotLogic, 1);

    cout<<"Testing XOR Logic with 2 inputs"<<endl;
    pLogicProcessor->ShowTruthTable(LogicProcessor::EXorLogic, 2);
    
    cout<<"Testing AND Logic with 3 inputs"<<endl;
    pLogicProcessor->ShowTruthTable(LogicProcessor::EAndLogic, 3);

    cout<<"Testing OR Logic with 4 inputs"<<endl;
    pLogicProcessor->ShowTruthTable(LogicProcessor::EOrLogic, 4);

    cout<<"Testing IMLIES Logic with 2 inputs"<<endl;
    pLogicProcessor->ShowTruthTable(LogicProcessor::EImpliesLogic, 2);

    delete pLogicProcessor;
}

/**************************************************************************
 Expression Evaluation Test
 You can evaluate your own expression:
 Here, This Function Evaluates expression (x & y) | (y ^ !x)
 **************************************************************************/
void ExpressionEvalTest1(bool x, bool y) {
    BoolLogic xBool(x);
    BoolLogic yBool(y);

    NotLogic xNot(xBool);
    AndLogic xyAnd(xBool, yBool);
    XorLogic yxXor(yBool, xNot);
    OrLogic allOr(xyAnd, yxXor);

    cout<<endl<<"Value of Expression is : "<<allOr.Process()<<endl;
}

/**************************************************************************
 Expression Evaluation Test
 You can evaluate your own expression:
 Here, This Function Evaluates expression (x & y) | (y ^ !x)
 **************************************************************************/
void ExpressionEvalTest2(bool x, bool y) {
    NotLogic not1(&x);
    bool andInput[2] = {x, y};
    AndLogic and1(andInput);
    bool xorInput[2] = {y, not1.Process()};
    XorLogic xor1(xorInput);
    bool orInput[2] = {and1.Process(), xor1.Process()};
    OrLogic or1(orInput);
    cout<<endl<<"Value of Expression is : "<<or1.Process()<<endl;
}

/**************************************************************************
 Symmetry Expression Evaluation Test
 This Function Reads the statically defined testCases and Tests for Processing
 **************************************************************************/
void SymmetryEvalTest(const char** aMatrix, int aNumInputs) {
    LogicProcessor* pLogicProcessor = new LogicProcessor;

    pLogicProcessor->IsSymmetryExpression(aMatrix, aNumInputs);
    delete pLogicProcessor;
}

int main() {
    //Truth Table Test
    TruthTableGenTest();
    
    //Expression Test - Part-I
    ExpressionEvalTest1(true, false);
    ExpressionEvalTest1(true, true);
    ExpressionEvalTest1(false, false);
    ExpressionEvalTest1(false, true);

    //Expression Test - Part-II
    ExpressionEvalTest2(true, false);
    ExpressionEvalTest2(true, true);
    ExpressionEvalTest2(false, false);
    ExpressionEvalTest2(false, true);

    //Symmetry Test (All Symmetry)
    SymmetryEvalTest(testCases1, 8);
    SymmetryEvalTest(testCases2, 8);
    SymmetryEvalTest(testCases3, 8);
    SymmetryEvalTest(testCases4, 8);
    SymmetryEvalTest(testCases5, 8);
    SymmetryEvalTest(testCases6, 8);
    SymmetryEvalTest(testCases7, 8);
    SymmetryEvalTest(testCases8, 8);

    // Horizontal symmetry only
    SymmetryEvalTest(testCases9, 8);
    SymmetryEvalTest(testCases10, 8);
    SymmetryEvalTest(testCases11, 8);
    SymmetryEvalTest(testCases12, 8);
    SymmetryEvalTest(testCases13, 8);
    SymmetryEvalTest(testCases14, 8);
    SymmetryEvalTest(testCases15, 8);
    SymmetryEvalTest(testCases16, 8);

    // Vertical symmetry only
    SymmetryEvalTest(testCases17, 8);
    SymmetryEvalTest(testCases18, 8);
    SymmetryEvalTest(testCases19, 8);
    SymmetryEvalTest(testCases20, 8);
    SymmetryEvalTest(testCases21, 8);
    SymmetryEvalTest(testCases22, 8);
    SymmetryEvalTest(testCases23, 8);
    SymmetryEvalTest(testCases24, 8);

    // No symmetry 
    SymmetryEvalTest(testCases25, 8);
    SymmetryEvalTest(testCases26, 8);
    SymmetryEvalTest(testCases27, 8);
    SymmetryEvalTest(testCases28, 8);
    SymmetryEvalTest(testCases29, 8);
    SymmetryEvalTest(testCases30, 8);
    SymmetryEvalTest(testCases31, 8);
    SymmetryEvalTest(testCases32, 8);

    return 0;
}
