//https://curl.haxx.se/libcurl/c/getinfo.html
//https://curl.haxx.se/libcurl/c/httpcustomheader.html

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <curl/curl.h>
#include <sys/time.h>
 
using namespace std;

const char* const LengthField = "Content-Length: ";
const char* const TypeField = "Content-Type: ";

bool FindField(const char* pHeader, size_t uSize, const char* const pField, string* pVal)
{
    string headerField(pHeader, uSize);
    size_t uFieldLen = 0;
    size_t index = 0;
    bool   bSuccess = false;

    index = headerField.find(pField);
    if (index != string::npos) {
        uFieldLen = strlen(pField);
        *pVal = headerField.substr(index + uFieldLen, uSize - uFieldLen);
        pVal->resize(pVal->size() - 2);
        bSuccess = true;
    }

    return bSuccess;
}
    
size_t gContentLen = 0;
string gContentType;

static size_t header_callback(char* buffer, size_t size,
                              size_t nitems, void* userdata)
{
    size_t uTotalLen = 0;
    size_t uVal = 0;
    string field;

    /* received header is nitems * size long in 'buffer' NOT ZERO TERMINATED */
    /* 'userdata' is set with CURLOPT_HEADERDATA */
    uTotalLen = nitems * size;
    //printf("header_callback: %zu * %zu = %zu: %s",  nitems, size, uTotalLen, buffer);
    printf("%s", buffer);

    if (FindField(buffer, uTotalLen, LengthField, &field)) {
        gContentLen = atoi(field.c_str());
        //printf("header_callback: %s [%s][%zu]\n", LengthField, field.c_str(), gContentLen);
    } else if (FindField(buffer, uTotalLen, TypeField, &field)) {
        gContentType = field;
        //printf("header_callback: %s [%s]\n", TypeField, field.c_str());
    }
    return uTotalLen;
}

static size_t body_callback(char* buffer, size_t size,
                              size_t nitems, void* userdata)
{
    /* received body is nitems * size long in 'buffer' NOT ZERO TERMINATED */
    /* 'userdata' is set with CURLOPT_HEADERDATA */
    //printf("body_callback: %zu: [%s]\n",  nitems * size, buffer);
    //printf("%s",  buffer);
    return nitems * size;
}

int curl_testCode(int type)
{
    CURL* curl = NULL;
    CURLcode res = CURLE_OK;
    struct timespec begin, end;

    printf("========================================================\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &begin);
    curl = curl_easy_init();
    if (curl) {
        //curl_easy_setopt(curl, CURLOPT_URL, "http://www.example.com/");
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com/");
        //curl_easy_setopt(curl, CURLOPT_URL, "https://blankslate.io/");
        //curl_easy_setopt(curl, CURLOPT_URL, "https://curl.haxx.se/libcurl/c/getinfo.html");
        if (type == 0) {
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
        } else {
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
        }
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, body_callback);
        res = curl_easy_perform(curl);
 
        if (CURLE_OK == res) {
            char* ct = NULL;
            char* url = NULL;
            long  response_code = 0;
            /* ask for the content-type */ 
            res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
            if ((CURLE_OK == res) && ct) {
                printf("We received Content-Type: %s\n", ct);
            }

            res = curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);
            if ((CURLE_OK == res) && url) {
                printf("We received URL: %s\n", url);
            }
            res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

            printf("Content-Type: [%s]\n", gContentType.c_str());
            printf("Content-Length: [%zu]\n", gContentLen);
            printf("HTTPS Response Code [%ld]\n", response_code);
        }
 
        /* always cleanup */ 
        curl_easy_cleanup(curl);
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    unsigned long delta_us = (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_nsec - begin.tv_nsec) / 1000;
    printf("took %lu micro-seconds\n", delta_us);
    printf("========================================================\n\n");
    return 0;
}

void str_testCode()
{
    long long contentTotalLength = 0;
    size_t  index = 0;
    string  name;
    string  value;
    string  value_;
    //string  info("Content-Range: bytes 12345-23456/78832345678\n\r");
    string  info("Content-Range: bytes 12345-23456/*\n\r");
    //string  info("Range: bytes=0-");
    //string  info("Connection: keep-alive");

    index = info.find(": ");
    if (index > 0) {
        name = info.substr(0, index);
        //value = info.substr(index + 2, info.size());
        value = info.substr(index + 2, string::npos);
        printf("Name: [%s] Value: [%s]\n", name.c_str(), value.c_str());
    }
    index = value.rfind('/');
    if (index != string::npos) {
        value_ = value.substr(index + 1);
        value_.resize(value_.size() - 2); //Ignore "\r\n"
        //contentTotalLength = stoll(value_);
        contentTotalLength = strtol(value_.c_str(), NULL, 0);
        printf("Str-Val: [%s] Value: [%lld]\n", value_.c_str(), contentTotalLength);
    }
}

#define TAG "GIRI TEST"
#define LOG(TAG, ...) printf("%s:%s: ", TAG, __FUNCTION__); \
                      printf(__VA_ARGS__);                  \
                      printf("\n");

void alloc_test()
{
    LOG(TAG, "test");
    char* p = (char*)malloc(0x8000);
    if (p) {
        strcpy(p, "Girish S");
        printf("%p:%s\n", p, p);
        p = (char*)realloc(p, 0xA000);
        if (p) {
            strcpy(p, "Girish Shetty S");
            printf("%p:%s\n", p, p);
        }
    }
}


int main (void)
{
    LOG(TAG, "giri");
    //curl_testCode(0);
    //curl_testCode(1);
    //str_testCode();
    alloc_test();
    return 0;
}
