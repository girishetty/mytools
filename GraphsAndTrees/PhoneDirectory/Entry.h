#ifndef _ENTRY_H
#define _ENTRY_H

#include <string>
#include <iostream>

class Node;

//Phonebook entry
class Entry {
friend class Node;

public:
  Entry() {}

  Entry(const std::string& aName, const std::string& aPhoneNum) : iName(aName), iPhoneNum(aPhoneNum) {}

  Entry(const char* aName, const char* aPhoneNum) {
    if (aName) {
      iName = aName;
    }
    if (aPhoneNum) {
      iPhoneNum = aPhoneNum;
    }
  }

  Entry(const Entry& anEntry) : iName(anEntry.iName), iPhoneNum(anEntry.iPhoneNum) {}

  ~Entry() {}

  void Display() const {
    std::cout << "Name:   " << iName << std::endl;
    std::cout << "Phone#: " << iPhoneNum << std::endl;
  }

  const char* Name() const {
    return iName.c_str();
  }

  const char* PhoneNum() const {
    return iPhoneNum.c_str();
  }

private:
  std::string iName;
  std::string iPhoneNum;
  //any other data follows here
};

#endif // _ENTRY_H
