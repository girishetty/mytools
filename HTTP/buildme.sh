rm -f a.out
gcc -std=c++11 https.c MediaHTTPConnection.cpp TestMediaHttp.cpp -lcurl -lstdc++ -lcrypto

