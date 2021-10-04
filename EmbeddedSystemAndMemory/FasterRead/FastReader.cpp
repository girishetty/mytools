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

bool FastReader::readIntoCache() {
  auto copied = mReader.read(mCache.data());
  if (copied < 0) {
    // Notify of read failure.
    return false;
  }
  mCacheAvailability = copied;
  mCachePosition = 0;
  if (copied < FourKReader::FourK) {
    // We have hit end of stream.
    mEOS = true;
  }
  return true;
}

size_t FastReader::readFromCache(uint8_t* buffer, size_t bufferSize) {
  auto toCopy = std::min(mCacheAvailability, bufferSize);
  if (toCopy > 0) {
    memcpy(buffer, mCache.data() + mCachePosition, toCopy);
    mCacheAvailability -= toCopy;
    if (mCacheAvailability == 0) {
      mCachePosition = 0;
    } else {
      mCachePosition += toCopy;
    }
  }

  return toCopy;
}

ssize_t FastReader::read(uint8_t* buffer, size_t bufferSize) {

  if (!buffer) {
    return -1;
  }
  if (mEOS) {
    return 0;
  }
  // Step#1: Check if we have any cached data available.
  auto toCopy = readFromCache(buffer, bufferSize);
  bufferSize -= toCopy;

  // Step#2: When bufferSize >= FourK
  // See if we have to make read call on the buffer directly
  while (bufferSize >= FourKReader::FourK) {
    auto copied = mReader.read(buffer + toCopy);
    if (copied < 0) {
      // Notify of read failure.
      return copied;
    }
    bufferSize -= copied;
    toCopy += copied;
    if (copied < FourKReader::FourK) {
      // We have hit end of stream.
      mEOS = true;
      break;
    }
  }

  // Step#3: When 0 < bufferSize < FourK
  // See if we have to read more data into our cache.
  if (!mEOS && bufferSize > 0) {
    // By now we know that whatever in the internal Cache has been consumed already.
    if (readIntoCache()) {
      toCopy += readFromCache(buffer + toCopy, bufferSize);
    } else {
      // Notify of read failure.
      return -1;
    }
  }

  cout << "FastReader::read(" << bufferSize << ") : " << toCopy << endl;
  return toCopy;
}
