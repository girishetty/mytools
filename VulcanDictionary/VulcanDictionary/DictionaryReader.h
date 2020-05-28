#ifndef _DECTIONARY_READER_H
#define _DECTIONARY_READER_H

#include "TypeDefines.h"

class AlphabetTable;

class DictionaryReader {
public:
    // Constructor that takes a text file (with wchar contents) that contains words in the Vulcan Dictionary.
    // All the alphabets in the dictionary are vulcan alphabets
    // Each line is a word
    DictionaryReader(const char* pDictionaryFile);

    //To Display Vulcan Alphabets in Lexicographical order
    void DisplayVulcanAlphabets() const;
    //To Display total number of Vulcan Alphabets in the Dictionary
    unsigned int TotalVulcanAlphabets() const;

private:
    //private routine that reads each words from the input file and sets up AlphabetTable
    void StartReading();

    //private routine that converts utf-8 encoding to multibyte char
    UINT ConvertUTF8ToMultiBytes(UCHAR currentByte, UCHAR nextByte) const;

    //private routine that converts utf-8 encoding to multibyte char
    UINT ConvertUTF8ToMultiBytes(UCHAR currentByte, UCHAR nextByte, UCHAR nextToNextByte) const;

    // Member variable that points to the Vulcan Dictionary Input File
    const char* iDictionaryFile;
    // AlphabetTable - Hash Table that contains list of all Vulcan Alphabets
    AlphabetTable* iTable;
    // Holds total number of unique Vulcan Alphabets in the Dictionary
    unsigned int iTotalAlphabets;
};

#endif //_DECTIONARY_READER_H
