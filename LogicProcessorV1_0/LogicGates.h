#ifndef LOGICGATES_H
#define LOGICGATES_H

#include <iostream>
using namespace std;

const int KMinInput = 2;
const char TRUE = 't';
const char FALSE = 'f';

/************************************************************************
 Abstract Base Class for All types of Logic
 This Interface has 3 pure virtual functions which will be implemented by
 all Logics with their specific Operation.

 ************************************************************************/

class Logic {

public:
    //virtual Destructer, which does nothing
    virtual ~Logic() { }

    //Pure Virtual functions which will be implemented by Sub classes
    //For Processing the inputs
    virtual const bool Process() = 0;
    virtual const bool Process(const bool* const aInput) = 0;
    //For Setting the input
    virtual void SetInputs(const bool* const aInput) = 0;

protected:
    //This API will be used by all Concreete subclases for setting inputs
    // SetInputs calls this API
    void SetInputsFrom(bool* aToInput, const bool* const aFromInput, int aNumInputs);
};


/***************************************************************************
 BoolLogic which has just hold a boolean Value
 ***************************************************************************/

class BoolLogic : public Logic {

public:
    //Constructer and destructer
    BoolLogic(bool aInput = true);
    BoolLogic(Logic& aInput);
    BoolLogic(Logic* const aInput);
    ~BoolLogic();

    //Implementation of Not Logic
    const bool Process();
    const bool Process(const bool* const aInput);

    void SetInputs(const bool* const aInput);

private:
    bool iInput;
};


/***************************************************************************
 NotLogic which has its own implementation of Base Class's virtual functions
 ***************************************************************************/

class NotLogic : public Logic {

public:
    //Constructer and destructer
    NotLogic();
    NotLogic(const bool* const aInput);
    NotLogic(Logic& aInput);
    NotLogic(Logic** const aInput);
    ~NotLogic();

    //Implementation of Not Logic
    const bool Process();
    const bool Process(const bool* const aInput);

    void SetInputs(const bool* const aInput);

private:
    bool iInput;
};


/***************************************************************************
 AndLogic which has its own implementation of Base Class's virtual functions
 ***************************************************************************/

class AndLogic : public Logic {
public:
    //Constructer and destructer
    AndLogic(int aNumInputs = KMinInput);
    AndLogic(const bool* const aInput, int aNumInputs = KMinInput);
    AndLogic(Logic& aInput1, Logic& aInput2);
    AndLogic(Logic** const aInput, int aNumInputs = KMinInput);
    ~AndLogic();
    
    //Implementation of And Logic
    const bool Process();
    const bool Process(const bool* const aInput);
    void SetInputs(const bool* const aInput);

private:
    int iNumInputs;
    bool* iInputs;
};

/***************************************************************************
 OrLogic which has its own implementation of Base Class's virtual functions
 ***************************************************************************/

class OrLogic : public Logic {
public:
    //Constructer and destructer
    OrLogic(int aNumInputs = KMinInput);
    OrLogic(const bool* const aInput, int aNumInputs = KMinInput);
    OrLogic(Logic& aInput1, Logic& aInput2);
    OrLogic(Logic** const aInput, int aNumInputs = KMinInput);
    ~OrLogic();

    //Implementation of Or Logic
    const bool Process();
    const bool Process(const bool* const aInput);
    void SetInputs(const bool* const aInput);

private:
    int iNumInputs;
    bool* iInputs;
};

/***************************************************************************
 XorLogic which has its own implementation of Base Class's virtual functions
 ***************************************************************************/

class XorLogic : public Logic {
public:
    //Constructer and destructer
    XorLogic(int aNumInputs = KMinInput);
    XorLogic(const bool* const aInput, int aNumInputs = KMinInput);
    XorLogic(Logic& aInput1, Logic& aInput2);
    XorLogic(Logic** const aInput, int aNumInputs = KMinInput);

    ~XorLogic();

    //Implementation of Xor Logic
    const bool Process();
    const bool Process(const bool* const aInput);
    void SetInputs(const bool* const aInput);

private:
    int iNumInputs;
    bool* iInputs;
};

/***************************************************************************
 ImpliesLogic which has its own implementation of Base Class's virtual functions
 ***************************************************************************/

class ImpliesLogic : public Logic {
public:
    //Constructer and destructer
    ImpliesLogic();
    ImpliesLogic(const bool* const aInput);
    ImpliesLogic(Logic& aInput1, Logic& aInput2);
    ImpliesLogic(Logic** const aInput);
    ~ImpliesLogic();

    //Implementation of Implies Logic
    const bool Process();
    const bool Process(const bool* const aInput);
    void SetInputs(const bool* const aInput);

private:
    bool* iInputs;
};

#endif //LOGICGATES_H
