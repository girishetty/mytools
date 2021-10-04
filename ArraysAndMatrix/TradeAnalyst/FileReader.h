#ifndef FILEREADER_H_INCLUDED
#define FILEREADER_H_INCLUDED

#include <fstream>

using namespace std;

class StreamReader {
public:
    StreamReader(const char* fName) : iFile(fName, ios::binary) {}
    StreamReader() {}
    ~StreamReader() { iFile.close(); }

    bool Open(const char* fName);

    bool ReadData(char* pData, uint32_t uDataLen);

    inline bool IsReadDone() {
        return iFile.eof();
    }

private:
    ifstream iFile;
};

#endif // FILEREADER_H_INCLUDED
