#ifndef _ENTRY_H
#define _ENTRY_H

#include <iostream>
using namespace std;
#include <string.h>

class Node;

//Phonebook entry
class Entry {
friend class Node;

public:
    Entry() : iName(NULL), iPhoneNum (NULL) { }

    Entry(const char* aName, const char* aPhoneNum) {
        CopyEntry(aName, aPhoneNum);
    }

    Entry(const Entry& anEntry) {
        CopyEntry(anEntry.iName, anEntry.iPhoneNum);
    }

    void CopyEntry(const char* aName, const char* aPhoneNum) {
        iName = NULL;
        iPhoneNum = NULL;
        if (aName) {
            iName = new char[strlen(aName) + 1];
            strcpy(iName, aName);
        }
        if (aPhoneNum) {
            iPhoneNum = new char[strlen(aPhoneNum) + 1];
            strcpy(iPhoneNum, aPhoneNum);
        }
    }

    ~Entry() {
        delete [] iName;
        delete [] iPhoneNum;
    }

    void Display() const {
        cout<<"Name:   "<<iName<<endl;
        cout<<"Phone#: "<<iPhoneNum<<endl;
    }

    const char* Name() const {
        return iName;
    }
    const char* PhoneNum() const {
        return iPhoneNum;
    }

private:
    char* iName;
    char* iPhoneNum;
    //any other data follows here
};

#endif // _ENTRY_H
