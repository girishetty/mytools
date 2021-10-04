#include <iostream>
#include "FileReader.h"

using namespace std;

bool StreamReader::Open(const char* fName) {
    //Close the file if its already open
    if (iFile.is_open()) {
        iFile.close();
    }
    //Open the file for reading binary mode
    iFile.open(fName, ios::binary);
    return iFile.good();
}

bool StreamReader::ReadData(char* pData, uint32_t uDataLen) {
    uint32_t uReadCount = 0;

    //Read uDataLen bytes of data and return true if we are successful reading those many bytes
    iFile.read(pData, uDataLen);
    uReadCount = iFile.gcount();
    return (uReadCount == uDataLen);
}
