#include <cstdlib>
#include <algorithm>
#include <iostream>

#include "FourKReader.h"

using std::cout;
using std::endl;

// Limit the size to 100 MB.
static const size_t kMaxFileSize = 100 * 1024 * 1024;

FourKReader::FourKReader(size_t maxSize) : mDataTotalize(maxSize) {
  if (mDataTotalize == 0) {
    mDataTotalize = static_cast<size_t>(std::rand());
    mDataTotalize = mDataTotalize % kMaxFileSize;
  }
}

ssize_t FourKReader::read(uint8_t* buffer) {
  ssize_t ret = -1;
  if (buffer) {
    ret = std::min((size_t)FourK, mDataTotalize - mDataPosition);
    cout << "FourKReader::read: " << ret << endl;
    mDataPosition += ret;
  }

  return ret;
}
