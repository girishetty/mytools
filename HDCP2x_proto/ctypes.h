
#ifndef _CTYPES_H_
#define _CTYPES_H_

//#define NULL (void*)0

typedef unsigned int  UNSG32;
typedef signed int    SIGN32;
typedef unsigned char UNSG8;
typedef signed int    STATUS;
typedef unsigned char UINT8;
typedef int           INT32;
typedef unsigned int  UINT32;
typedef unsigned int  BOOL;

typedef enum _Status {

    STATUS_SUCCESS = 0,
    STATUS_FAILURE = 255,
    STATUS_INVALID_ARGS,
} Status;

void hex_print(const char* what, const void* pv, int len);

#endif //
