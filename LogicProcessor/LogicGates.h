#ifndef LOGICGATES_H
#define LOGICGATES_H

#include <string>
#include <vector>

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

  Logic(int aNumInputs) : iNumInputs(aNumInputs) {
    CreateIntputTable();
  }

  //virtual Destructer, which does nothing
  virtual ~Logic() { }

  //Pure Virtual functions which will be implemented by Sub clasees
  virtual char GetOutput(const std::string& aInput) const = 0;

  // Print the truth table for this logical operation.
  void PrintTruthTable() const;
  
protected:
  //Two Utility Methods for creating and Deleting Tables
  void CreateIntputTable();

protected:
  int                      iNumInputs;
  std::vector<std::string> iInputTable;
};


/***************************************************************************
 NotLogic which has its own implementation of Base Class's virtual functions
 ***************************************************************************/

class NotLogic : public Logic {
public:
  NotLogic(int aNumInputs = 1) : Logic(aNumInputs) {}
  ~NotLogic() {}

  char GetOutput(const std::string& aInput) const override;
};


/***************************************************************************
 AndLogic which has its own implementation of Base Class's virtual functions
 ***************************************************************************/

class AndLogic : public Logic {
public:
  AndLogic(int aNumInputs = 2) : Logic(aNumInputs) {}
  ~AndLogic() {}

  char GetOutput(const std::string& aInput) const override;
};

/***************************************************************************
 OrLogic which has its own implementation of Base Class's virtual functions
 ***************************************************************************/

class OrLogic : public Logic {
public:
  OrLogic(int aNumInputs = 2) : Logic(aNumInputs) {}
  ~OrLogic() {}

  char GetOutput(const std::string& aInput) const override;
};

#endif //LOGICGATES_H
