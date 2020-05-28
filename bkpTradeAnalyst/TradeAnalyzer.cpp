#include <iostream>
#include <memory.h>
#include "TradeAnalyzer.h"

using namespace std;

int TradeAnalyzer::StartProcessing()
{
    TradeMessage message;
    bool readStatus = false;
    //cout<<sizeof(TradeHeader)<<":"<<sizeof(EntryMSG)<<":"<<sizeof(AckMSG)<<":"<<sizeof(FillMSG)<<endl;

    do
    {
        memset(&message, 0, sizeof(TradeMessage));
        readStatus = iInput.ReadData((char*)&message.iHeader, sizeof(TradeHeader));
        if (readStatus)
        {
            //message.iHeader.Display();
            if (message.iHeader.msg_type == EOrderEntry ||
                    message.iHeader.msg_type == EOrderACK ||
                    message.iHeader.msg_type == EOrderFill)
            {
                readStatus = iInput.ReadData(message.iMessage.iMsg, message.iHeader.msg_length);
                if (readStatus)
                {
                    //hex_dump("Data", (uint8_t*)&message, sizeof(TradeHeader) + message.iHeader.msg_length);
                    iList.Add(&message, sizeof(TradeHeader) + message.iHeader.msg_length);
                }
                else
                {
                    cout<<"Message Read Error!! Aborting"<<endl;
                }
            }
            else
            {
                cout<<"Invalid Message in the file, which we are ignoring"<<endl;
            }
        }
        else
        {
            cout<<"Consumed all the data Successfully"<<endl;
        }
    } while (readStatus);

    return 0;
}

void TradeAnalyzer::GetStatistics()
{
    cout<<"All Record Count: "<<iList.GetAllRecordCount()<<endl;
    cout<<"Entry Record Count: "<<iList.GetTradeRecordsCount(EOrderEntry)<<endl;
    cout<<"ACK   Record Count: "<<iList.GetTradeRecordsCount(EOrderACK)<<endl;
    cout<<"Fill  Record Count: "<<iList.GetTradeRecordsCount(EOrderFill)<<endl;

    const char* active_trader = iList.GetActiveTrader();
    if (active_trader) {
        cout<<"Most Active Trader order:"<<active_trader[0]<<active_trader[1]<<active_trader[2]<<endl;
    }
    const char* highLiquid_trader = iList.GetHighestLiquidTrader();
    if (highLiquid_trader) {
        cout<<"High Liquid Trader :"<<highLiquid_trader[0]<<highLiquid_trader[1]<<highLiquid_trader[2]<<endl;
    }
}
