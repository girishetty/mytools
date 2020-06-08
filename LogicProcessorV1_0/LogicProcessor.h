#ifndef LOGICPROCESSOR_H
#define LOGICPROCESSOR_H

/*************************************************************************************
 LogicProcessor class
 This class has a Factory Method as Creational Pattern for Creating different Logics
 It also a function for generating truth table for all Logic with any number of inputs
 *************************************************************************************/

//Forward Declaration
class Logic;

class LogicProcessor {

public:
    //Different Types of Logics
    enum LogicType{
        ENotLogic=0,
        EAndLogic,
        EOrLogic,
        EXorLogic,
        EImpliesLogic
    };

    //Different Types of Symmetry
private:
    enum SymmetryType {
        ENot = 0,
        EHorizontal = 0x1,
        EVerticle = 0x2,
        EAll = 0x3,
    };

public:
    //Factory Method for Creating Logic Objects
    Logic* CreateLogic(LogicType aLogicType, int aNumInputs);
    //Truth Table Generating function
    void ShowTruthTable(LogicType aLogicType, int aNumInputs);
    //Processing an expression to check whether its Symmetry Matrix
    SymmetryType IsSymmetryExpression(const char** aMatrix, int aNumInputs);

private:
    //Helper function for generating all possible combination of inputs
    bool** GenerateInputTable(int aNumInputs);
    //Helper function for displaying bool values as char
    void DisplayValue(bool aVal);
};

#endif //LOGICPROCESSOR_H
