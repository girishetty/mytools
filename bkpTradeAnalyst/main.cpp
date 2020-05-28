#include <iostream>
#include <fstream>
#include <string.h>
#include "TradeAnalyzer.h"
#include "TradeMessage.h"

using namespace std;

int main(int argc, char* argv[])
{
#if 1
    TradeAnalyzer tAnalyzer(argv[1]);
    tAnalyzer.StartProcessing();
    tAnalyzer.GetStatistics();
#else
    TradeMessage msg;
    TradeMessageList temp_;
    vector<TradeMessage> temp;
    temp.push_back(msg);
    temp.push_back(msg);
    temp.push_back(msg);
    temp.push_back(msg);
    temp.push_back(msg);
    msg.iHeader.msg_type = 1;
    uint8_t* pTerm = (uint8_t*)&msg;
    pTerm += sizeof(msg) - LEN_TERM_STRING;

    memcpy(pTerm, gTermString, LEN_TERM_STRING);
    temp_.Add(&msg, sizeof(msg));
#endif

    return 0;
}
