/*
Given a class which reads minimum of 4K of data or End of File, upon every read operation,
write an API that can read any arbitrary size of data.

Given:

class FourKReader {

public:
  // Reads minimum of 4K of data or End of File into buffer.
  // Returns:
  //  -1 if input is invalid or some error while reading
  //  min (FourK, remaining data available in the Reader)
  //
  // When the reader has less than 4K of data, the read will return
  // that as the size, indicating the user that it has hit end of file.
  // Any subsequent call to read will return 0.
  ssize_t read(char* buffer);
};

*/

#include <vector>
#include <iostream>
#include "FastReader.h"

using std::cout;
using std::endl;
using std::vector;

void readAtATime(size_t fileSize, size_t readSize) {
  FourKReader reader4K(fileSize);
  FastReader  reader(reader4K);
  vector<uint8_t> buffer(readSize);
  ssize_t readCount = 0;

  cout << "readAtATime(" << fileSize << " : " << readSize << ")" << endl;
  do {
    readCount = reader.read(buffer.data(), buffer.size());
    cout << "Read " << readCount << " bytes" << endl;
  } while (readCount > 0);
  cout << "======================================" << endl;
}

int main() {

  readAtATime(100 * 1024, 1 * 1024);
  readAtATime(100 * 1024, 5 * 1024);
  readAtATime(100 * 1024, 10 * 1024);

  return 0;
}
