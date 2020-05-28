#include <iostream>
#include <fstream>
#include "AlphabetTable.h"
#include "DictionaryReader.h"

using namespace std;

// Making an assumption that the size of biggest word in a Vulcan Dictionary is 256 
#define WORD_MAX_SIZE 256

DictionaryReader::DictionaryReader(const char* pDictionaryFile) : iDictionaryFile(pDictionaryFile), iTable(NULL), iTotalAlphabets(0) {
    (void)iTotalAlphabets;
    if(iDictionaryFile) {
        iTable = new AlphabetTable();
        // Constructor starts building the Table
        StartReading();
    }
}

void DictionaryReader::StartReading() {
    char word[WORD_MAX_SIZE + 1]; //including the delimiting character, which is newline in this case
    ifstream dictionary(iDictionaryFile);
    UINT alphabet = 0;

    while (dictionary.good()) {
        dictionary.getline(word, WORD_MAX_SIZE+1);
        int len = strlen(word);
        // Now start pushing individual Alphabets into our Table
        for(int index = 0; index < len; index ++) {
            alphabet = 0;
            // Check whether the Unicode char is represented by 1 byte (0000 to 007F) as 0xxxxxxx
            if((word[index] & 0x80) == 0) {
                // Only one byte
                alphabet = word[index];
            } 
            //Check whether the Unicode char is represented by 2 bytes (0080 to 07FF) as 110xxxxx 10xxxxxx
            else if((word[index] & 0xE0) == 0xC0) {
                // Two bytes for this Unicode Character, form the multibytes now
                alphabet = ConvertUTF8ToMultiBytes(word[index], word[index+1]);
                // We have already read next byte
                index++;
            }
            //Check whether the Unicode char is represented by 3 bytes (0800 to FFFF) as 1110xxxx 10xxxxxx 10xxxxxx
            else if((word[index] & 0xF0) == 0xE0) {
                // Three bytes for this Unicode Character, form the multibytes now
                alphabet = ConvertUTF8ToMultiBytes(word[index], word[index+1], word[index+2]);
                // We have already read next 2 bytes
                index += 2;
            }
            else {
                // We are ignoring this
                // But the requirement changes to handle more Alphabets, that can be added here. 
                // That's why alphabet is unsigned int
            }
            iTable->AddAlphabet(alphabet);
        }
    }

    dictionary.close();
}

unsigned int DictionaryReader::ConvertUTF8ToMultiBytes(UCHAR currentByte, UCHAR nextByte) const {
    UINT alphabet = 0;
    // Two bytes for this Unicode Character
    UCHAR byte1 = 0;
    UCHAR byte2 = 0;
    // Decode the first byte ( 110BBBxx - should become 00000BBB)
    byte1 = (currentByte >> 2) & 0x07;
    // Decode the second byte (move 2 lsb bits from the current byte as the 2 msb bits and OR with the next byte
    // byte 2 is 110xxxBB - 10bbbbbb (next byte) should become BBbbbbbb
    byte2 = (currentByte << 6) | (nextByte & 0x3F);
    // Form the multibytes now
    alphabet = byte1;
    alphabet = (alphabet << 8) | byte2;
    return alphabet;
}

unsigned int DictionaryReader::ConvertUTF8ToMultiBytes(UCHAR currentByte, UCHAR nextByte, UCHAR nextToNextByte) const {
    UINT alphabet = 0;
    // Three bytes for this Unicode Character, but decoded character needs only 2 bytes
    UCHAR byte1 = 0;
    UCHAR byte2 = 0;
    // use the currentByte along with the next byte to create the first byte
    // 1110BBBB - 10bbbbxx (next byte) should become BBBBbbbb
    byte1 = (currentByte << 4) | ((nextByte >> 2) & 0x0F);
    // Form the the second byte with the next byte and next-to-next byte
    // byte 2 is 10xxxxBB (next byte) - 10bbbbbb (next+1 byte) should become BBbbbbbb
    byte2 = (nextByte << 6) | (nextToNextByte & 0x3F);
    // Form the multibytes now
    alphabet = byte1;
    alphabet = (alphabet << 8) | byte2;
    return alphabet;
}

void DictionaryReader::DisplayVulcanAlphabets() const {
    if(iTable) {
        iTable->DisplayTable();
    } else {
        cout<<"Dictionary Input File not available!!"<<endl;
    }
}

unsigned int DictionaryReader::TotalVulcanAlphabets() const {
    unsigned int count = 0;
    if(iTable) {
        count = iTable->TotalAlphabets();
    } else {
        cout<<"Dictionary Input File not available!!"<<endl;
    }
    return count;
}
