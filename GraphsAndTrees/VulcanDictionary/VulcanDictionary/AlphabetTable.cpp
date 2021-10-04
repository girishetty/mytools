#include <assert.h>
#include <iostream>
#include "AlphabetTable.h"

using namespace std;


void AlphabetTable::AddAlphabet(UINT anAlphabet) {
    unsigned int index = HashIndex(anAlphabet);
    //Now set that particular entry.
    /*
     * We can use this table to count no of occurances of each Alphabets, 
     * by incrementing the count for every occurance
     * in which case we will have to increment iTable[index] entry for every occurance
     * And also, as each entry can have a value of MAX 256 (as its UCHAR), we will have to change the data-type of Table so that it can hold up a bigger count (like long long)
     */
    if(iTable[index] == 0) {
        //We don't have an entry yet. So enter it and increment the number of unique Alphabets
        iTotalAlphabets++;
        //Set the entry to notify we have got an Alphabet
        iTable[index] = 1;
    }
}

void AlphabetTable::DisplayTable() const {
    cout<<"There are total of "<<iTotalAlphabets<<" unique Vulcan Alphabets in the Dictionary"<<endl;
    cout<<"Here is the list: "<<endl;
    for(UINT index = 0; index < iSize; index++) {
        if(iTable[index] != 0) {
            /* 
             * We have an entry here.
             * And the Alphabet at this entry is same as the index - as thats how we generated the hash-index
             * As we are representing the UTF-8 characters as multibye arrays, we can use wchar to print them
             */
            wchar_t ch = index;
            wcout << ch << " ";
        }
    }
    cout<<endl;
}

unsigned int AlphabetTable::HashIndex(UINT anAlphabet) const {
    //use the UTF-8 value of the character (as in mutlibyte) as the hash index
    UINT index = anAlphabet;
    //to make sure we are within the range
    assert(index < iSize);
    return index;
}
