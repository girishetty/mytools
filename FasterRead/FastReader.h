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
#include "FourKReader.h"

using std::vector;

class FastReader {
public:

  FastReader(FourKReader& reader) : mCache(FourKReader::FourK), mReader(reader) {}

  // Reads minimum of "bufferSize" or End of File into buffer.
  // Returns:
  //  -1 if input is invalid or some error while reading
  //  min (bufferSize, remaining data available in the Reader)
  //
  // When the reader has less than "bufferSize" of data, the read will return
  // that as the size, indicating the user that it has hit end of file.
  // Any subsequent call to read will return 0.
  ssize_t read(uint8_t* buffer, size_t bufferSize);

  bool isEOS() const { return mEOS; }

private:
  bool   readIntoCache();
  size_t readFromCache(uint8_t* buffer, size_t bufferSize);

private:
  bool            mEOS = false;
  size_t          mCachePosition = 0;
  size_t          mCacheAvailability = 0;
  vector<uint8_t> mCache;
  FourKReader&    mReader;
};
