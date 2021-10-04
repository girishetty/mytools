#pragma once

#include <sys/types.h>
#include <cstddef>

/*
 Class which reads minimum of 4K of data or End of File, upon every read operation.
};

*/

class FourKReader {
public:

  enum {
    FourK = 4 * 1024,
  };

  FourKReader(size_t maxSize);

  // Reads minimum of 4K of data or End of File into buffer.
  // Returns:
  //  -1 if input is invalid or some error while reading
  //  min (FourK, remaining data available in the Reader)
  //
  // When the reader has less than 4K of data, the read will return
  // that as the size, indicating the user that it has hit end of file.
  // Any subsequent call to read will return 0.
  ssize_t read(uint8_t* buffer);

private:
  size_t mDataTotalize = 0;
  size_t mDataPosition = 0;
};

