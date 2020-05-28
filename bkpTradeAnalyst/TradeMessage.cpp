
#include <string.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "TradeMessage.h"

using namespace std;

const char* const gTermString = "DBDBDBDB";

#ifdef _DEBUG
#include <stdio.h>

void TradeHeader::Display() const {
    cout<<"marker: "<<marker<<endl;
    cout<<"Msg Type: "<< +msg_type<<endl;
    cout<<"Sequence ID: "<<sequence_id<<endl;
    cout<<"Time-stamp: "<<timestamp<<endl;
    cout<<"Msg Dir: "<<msg_direction<<endl;
    cout<<"Msg Len: "<<msg_length<<endl;
    cout<<"=================================="<<endl;
}

void EntryMSG::Display() const {
    cout<<"Price: "<<price<<endl;
    cout<<"Qty: "<<qty<<endl;
    cout<<"Instrument: "<<instrument<<endl;
    cout<<"Side: "<< +side<<endl;
    cout<<"Client ID: "<<client_assigned_id<<endl;
    cout<<"TIF: "<<+time_in_force<<endl;
    cout<<"Trade Tag: "<<trader_tag<<endl;
    cout<<"Firm ID: "<< +firm_id<<endl;
    cout<<"Firm: "<< firm<<endl;
    cout<<"=================================="<<endl;
}

void FillMSG::Display() const {
    cout<<"Order ID: "<<order_id<<endl;
    cout<<"Fill Price: "<<fill_price<<endl;
    cout<<"Fill Qty: "<<fill_qty<<endl;
    cout<<"No of Contras: "<< +no_of_contras<<endl;
    for (int i=0; i<no_of_contras; i++) {
        cout<<"Firm ID: "<< +firmInfo[i].firm_id<<endl;
        cout<<"Trader TAG: "<<firmInfo[i].trader_tag<<endl;
        cout<<"Qty: "<<firmInfo[i].qty<<endl;
    }
    cout<<"=================================="<<endl;
}

void hex_dump(const char* name, const uint8_t* data, const int uLen)
{
    int index = 0;
    printf("%s: (%d) : \n", name, uLen);
    for(; index<uLen; )
    {
        printf("0x%02X ", data[index]);
        index++;
        if (index % 16 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}

#endif // _DEBUG

//Adds one Trade transaction record to the List
void TradeMessageList::Add(TradeMessage* pMsg, uint32_t uLen)
{
    //Look for the termination string before doing anything
    const uint8_t* pTerm = (const uint8_t*)pMsg;
    pTerm += uLen - LEN_TERM_STRING;
    if (memcmp(pTerm, gTermString, LEN_TERM_STRING) == 0) {
        switch (pMsg->iHeader.msg_type) {
        case EOrderEntry:
            //pMsg->iHeader.Display();
            //pMsg->iMessage.iEntry.Display();
            iOrderEntryList.push_back(*pMsg);
            break;
        case EOrderACK:
            iOrderAckList.push_back(*pMsg);
            break;
        case EOrderFill:
            iOrderFillList.push_back(*pMsg);
            break;
        }
    }
    else {
        cout<<"No Termination String!!! Ignoring this Data Record"<<endl;
    }
}

//Gets the count of all Trade transaction records
uint32_t TradeMessageList::GetAllRecordCount() const
{
    uint32_t sum = iOrderEntryList.size() + iOrderAckList.size() + iOrderFillList.size();

    return sum;
}

//Gets the count of individual Trade transaction records
uint32_t TradeMessageList::GetTradeRecordsCount(TradeMessageType aType) const
{
    uint32_t count = 0;

    switch(aType) {
    case EOrderEntry:
        count = iOrderEntryList.size();
        break;
    case EOrderACK:
        count = iOrderAckList.size();
        break;
    case EOrderFill:
        count = iOrderFillList.size();
        break;
    }

    return count;
}

//Comparison function for Order Entry records based on Client ID
bool CompareOrderEntry(const TradeMessage& lhs, const TradeMessage& rhs) {
    return lhs.iMessage.iEntry.client_assigned_id < rhs.iMessage.iEntry.client_assigned_id;
}

//Comparison function for Order ACK records based on Client ID
bool CompareOrderAck(const TradeMessage& lhs, const TradeMessage& rhs) {
    return lhs.iMessage.iAck.client_id < rhs.iMessage.iAck.client_id;
}

//Comparison function for Order Fill records based on Quantity of Trade
bool CompareOrderFill(const TradeMessage& lhs, const TradeMessage& rhs) {
    return lhs.iMessage.iFill.fill_qty > rhs.iMessage.iFill.fill_qty ;
}

struct ClientOrderList {
    uint64_t client_id;
    uint32_t total_qty;

    ClientOrderList() : client_id(0), total_qty(0) {};
};

const char* TradeMessageList::GetActiveTrader()
{
    uint32_t index = 0;
    uint32_t index_ = 0;
    int32_t  salesIndex = -1;
    uint32_t order_id = 0;
    uint32_t order_id_ = 0;
    uint32_t fill_qty = 0;
    const char* pActiveTrader = NULL;

    uint32_t totalSalesRecords = iOrderFillList.size();
    uint32_t totalTradeRecords = iOrderAckList.size();
    ClientOrderList maxTrade;
    uint64_t client_id = 0;

    //Create a temporary list
    ClientOrderList* pClientOrderList = new ClientOrderList[totalTradeRecords];
    memset(pClientOrderList, 0, sizeof(ClientOrderList) * totalTradeRecords);

    //Sort the Order ACK list based on Client ID
    sort(iOrderAckList.begin(), iOrderAckList.end(), CompareOrderAck);

    //Find the total Trades made for individual clients using Order Fill record and save in the list
    for(index = 0; index < totalTradeRecords; index++) {
        if (client_id != iOrderAckList[index].iMessage.iAck.client_id) {
            client_id = iOrderAckList[index].iMessage.iAck.client_id;
            salesIndex++;
            pClientOrderList[salesIndex].client_id = client_id;
            pClientOrderList[salesIndex].total_qty = 0;

            //See whether this client has the max trade quantity
            if (salesIndex == 0) {
                maxTrade = pClientOrderList[0];
            } else if (maxTrade.total_qty < pClientOrderList[salesIndex-1].total_qty) {
                maxTrade = pClientOrderList[salesIndex - 1];
            }
        }
        order_id = iOrderAckList[index].iMessage.iAck.order_id;
        fill_qty = 0;

        //Using the Trade Order Fill records, get the trade quantity for each order
        for(index_= 0; index_ < totalSalesRecords; index_++){
            order_id_ = iOrderFillList[index_].iMessage.iFill.order_id;

            if(order_id == order_id_) {
                fill_qty = iOrderFillList[index_].iMessage.iFill.fill_qty;
                break;
            }
        }
        //Add up the order quantity for this client
        pClientOrderList[salesIndex].total_qty += fill_qty;
    }

    //Compare with the last item for record with max trade quantity
    if (maxTrade.total_qty < pClientOrderList[salesIndex].total_qty) {
        maxTrade = pClientOrderList[salesIndex];
    }

    //Now use the Order Entry record to find the Trader Tag based on the Client ID of the maximum Trades made
    totalTradeRecords = iOrderEntryList.size();
    for (index = 0; index < totalTradeRecords; index++) {
        if (maxTrade.client_id == iOrderEntryList[index].iMessage.iEntry.client_assigned_id) {
            pActiveTrader = iOrderEntryList[index].iMessage.iEntry.trader_tag;
            break;
        }
    }

    delete pClientOrderList;
    return pActiveTrader;
}

struct ClientOrderEntryList {
    const char*     trader_tag;
    uint64_t total_liquidity;

    ClientOrderEntryList() : trader_tag(NULL), total_liquidity(0) {}
};

const char* TradeMessageList::GetHighestLiquidTrader()
{
    uint32_t index = 0;
    uint32_t ttif = 0;
    uint32_t totalTradesRecords = iOrderEntryList.size();
    uint64_t client_id = 0;
    ClientOrderEntryList maxClientOrderList;
    ClientOrderEntryList clientOrderList;

    //Sort the Order Entry list based on Client ID
    sort(iOrderEntryList.begin(), iOrderEntryList.end(), CompareOrderEntry);

    //Find the cumulative trading amount for individual clients
    for(; index < totalTradesRecords; index++) {
        ttif = iOrderEntryList[index].iMessage.iEntry.time_in_force;
        if (client_id != iOrderEntryList[index].iMessage.iEntry.client_assigned_id && ttif == EGFD) {
            client_id = iOrderEntryList[index].iMessage.iEntry.client_assigned_id;
            clientOrderList.trader_tag =  iOrderEntryList[index].iMessage.iEntry.trader_tag;
            clientOrderList.total_liquidity =
              iOrderEntryList[index].iMessage.iEntry.qty * iOrderEntryList[index].iMessage.iEntry.price;
            if (maxClientOrderList.total_liquidity < clientOrderList.total_liquidity) {
                maxClientOrderList = clientOrderList;
            }
        } else if (ttif == EGFD) {
            clientOrderList.total_liquidity +=
              iOrderEntryList[index].iMessage.iEntry.qty * iOrderEntryList[index].iMessage.iEntry.price;
            //See whether this client has the max trade quantity
            if (maxClientOrderList.total_liquidity < clientOrderList.total_liquidity) {
                maxClientOrderList = clientOrderList;
            }
        }
    }
#ifdef _DEBUG
    cout<<"Highest Liquid Trader amount: "<<maxClientOrderList.total_liquidity<<endl;
#endif // _DEBUG
    return maxClientOrderList.trader_tag;
}
