#ifndef LOGICGATES_H
#define LOGICGATES_H

#include <iostream>
using namespace std;

const char TRUE = 't';
const char FALSE = 'f';

/************************************************************************
 Abstract Base Class for All types of Logic
 This Interface has two pure virtual functions which will be implemented
 all Logics with their specific Operation.

 It also provides two functionalities which is used for creating table of
 all possible TRUTH combinations for given number of inputs
 ************************************************************************/

class Logic {
public:

    //virtual Destructer, which does nothing
    virtual ~Logic() { }

    //Pure Virtual functions which will be implemented by Sub clasees
    virtual void GetTruthTable() = 0;
    virtual const char GetOutput(char* aInput, int aCount) = 0;
    
protected:
    //Two Utility Methods for creating and Deleting Tables
    char** GetIntputTable(int aInput = 1);
    void DeleteIntputTable(char** aTable, int aInput = 1);
};


/***************************************************************************
 NotLogic which has its own implementation of Base Class's virtual functions
 ***************************************************************************/

class NotLogic : public Logic {
public:
    NotLogic(int aNumInputs = 1);
    ~NotLogic();
    void GetTruthTable();
    const char GetOutput(char* aInput, int aCount = 1);

private:
    int iNumInputs;
    char** iInputTable;
};


/***************************************************************************
 AndLogic which has its own implementation of Base Class's virtual functions
 ***************************************************************************/

class AndLogic : public Logic {
public:
    AndLogic(int aNumInputs = 2);
    ~AndLogic();
    void GetTruthTable();
    const char GetOutput(char* aInput, int aCount = 2);

private:
    int iNumInputs;
    char** iInputTable;
};

/***************************************************************************
 OrLogic which has its own implementation of Base Class's virtual functions
 ***************************************************************************/

class OrLogic : public Logic {
public:
    OrLogic(int aNumInputs = 2);
    ~OrLogic();
    void GetTruthTable();
    const char GetOutput(char* aInput, int aCount = 2);

private:
    int iNumInputs;
    char** iInputTable;
};

#endif //LOGICGATES_H
