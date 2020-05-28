#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define AES_KEYSIZE      16                                                                                         
#define TWOK_BYTES       2048

void random_fun() {
    int uDataSize = 0;

    srand(time(NULL));
    //Generate a 32 bit random number for data size 
    uDataSize = rand();
    printf("random_fun: %x\n", uDataSize);
    //Make sure data size is less than TWOK_BYTES and aligned with AES_KEYSIZE
    uDataSize = (uDataSize & (TWOK_BYTES - 1)) & ~(AES_KEYSIZE - 1);
    printf("random_fun: %x\n", uDataSize);
    //uDataSize = (uDataSize) & ~(AES_KEYSIZE - 1);
    //printf("random_fun: %x\n", uDataSize);
    if (uDataSize == 0) {
        uDataSize = TWOK_BYTES;
    } 
    printf("random_fun: %x\n\n\n", uDataSize);
}

void getTime() {
    time_t rawtime;
    time_t mk_time;
    struct tm* timeinfo = NULL;

    /* get current timeinfo and modify it to the user's choice */
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
#if 0
    timeinfo->tm_year = year - 1900;
    timeinfo->tm_mon = month - 1;
    timeinfo->tm_mday = day;
#endif

    /* call mktime: timeinfo->tm_wday will be set */
    mk_time = mktime ( timeinfo );

    printf ("year[%d] mon[%d] day[%d]\n", timeinfo->tm_year, timeinfo->tm_mon, timeinfo->tm_mday);
    printf("Raw time: %ld\n", rawtime);
    printf("Mk time: %ld\n", mk_time);
}

int main ()
{
    random_fun();
    //getTime();
    //printf("Sleeping for 2 seconds\n");
    //sleep(2);
    //printf("Slept 2 seconds\n");
    //getTime();
    return 0;
}
