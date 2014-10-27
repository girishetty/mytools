#include <iostream>
#include <memory.h>
#include "TradeAnalyzer.h"

using namespace std;

int TradeAnalyzer::StartProcessing()
{
    bool readStatus = false;
    TradeMessage* pTM = NULL;
    uint32_t uMsgLen = 0;
    char header[TRADE_HEADER_LEN];
    char message[512];

    do {
        readStatus = iInput.ReadData(header, TRADE_HEADER_LEN);
        if (readStatus) {
            hex_dump("Header", (uint8_t*)header, TRADE_HEADER_LEN);
            pTM = TradeMessage::CreateInstance((uint8_t*)header, TRADE_HEADER_LEN);
            if (pTM) {
                //pTM->DisplayHeader();
                uMsgLen = pTM->MessageLength();
                readStatus = iInput.ReadData(message, uMsgLen);
                if (readStatus) {
                    hex_dump("Message", (uint8_t*)message, uMsgLen);
                    pTM->SetMessage((uint8_t*)message, uMsgLen);
                    iList.Add(pTM);
                } else {
                    cout<<"Message Read Error!! Aborting"<<endl;
                }
            } else {
                cout<<"Invalid Message in the file, which we are ignoring"<<endl;
            }
        }
    } while (readStatus);

    return 0;
}

void TradeAnalyzer::GetStatistics()
{
    cout<<iList.GetAllRecordCount()<<", "<<iList.GetTradeRecordsCount(EOrderEntry)<<", ";
    cout<<iList.GetTradeRecordsCount(EOrderACK)<<", "<<iList.GetTradeRecordsCount(EOrderFill)<<", ";

    const char* active_trader = iList.GetActiveTrader();
    if (active_trader) {
        cout<<active_trader<<", ";
    }
    const char* highLiquid_trader = iList.GetHighestLiquidTrader();
    if (highLiquid_trader) {
        cout<<highLiquid_trader;
    }
    iList.GetTradesPerInstrument();
}
