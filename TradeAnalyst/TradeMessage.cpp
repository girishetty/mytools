
#include <string.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "order32.h"
#include "TradeMessage.h"

using namespace std;

#ifdef _DEBUG

#include <stdio.h>

void hex_dump(const char* name, const uint8_t* data, const int uLen)
{
    int index = 0;
    printf("%s: (%d) : \n", name, uLen);
    for(; index<uLen; ) {
        printf("0x%02X ", data[index]);
        index++;
        if (index % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}
#endif // _DEBUG

#define TRADE_ENTRY_MSG_MIN_LEN   36
#define TRADE_ACK_MSG_LEN         14
#define TRADE_FILL_MSG_MIN_LEN    17
#define TRADE_TERM_STRINGLEN      8

const char* const gTermString = "DBDBDBDB";

TradeMessage* TradeMessage::CreateInstance(const uint8_t* pHeader, uint32_t uLen)
{
    TradeMessage* pTM = NULL;
    uint8_t val = 0;

    //Make sure the header if of right length
    if (uLen == TRADE_HEADER_LEN) {
        val = *(pHeader + sizeof(uint16_t));
        switch (val) {
        case EOrderEntry:
            pTM = new EntryMessage(pHeader, uLen);
            break;
        case EOrderACK:
            pTM = new AckMessage(pHeader, uLen);
            break;
        case EOrderFill:
            pTM = new FillMessage(pHeader, uLen);
            break;
        default:
            cout<<"ERROR: Cant identify this Trade Message Type!!"<<endl;
            pTM = NULL;
        }
    }

    return pTM;
}

inline const uint8_t* ReadUINT64(const uint8_t* pMsg, uint64_t* pVal)
{
    uint64_t val = *(uint64_t*)pMsg;

    if (O32_HOST_ORDER == O32_BIG_ENDIAN) {
        uint8_t* pTemp = (uint8_t*)&val;
        uint8_t  byte = 0;
        int index = 0;
        int index_ = sizeof(uint64_t) - 1;
        for (; index < index_; index++, index_--) {
            byte = pTemp[index_];
            pTemp[index_] = pTemp[index];
            pTemp[index] = byte;
        }
    }
    *pVal = val;
    pMsg += sizeof(uint64_t);

    return pMsg;
}

inline const uint8_t* ReadUINT32(const uint8_t* pMsg, uint32_t* pVal)
{
    uint32_t val = *(uint32_t*)pMsg;

    if (O32_HOST_ORDER == O32_BIG_ENDIAN) {
        uint8_t* pTemp = (uint8_t*)&val;
        uint8_t  byte = 0;
        byte = pTemp[0];
        pTemp[0] = pTemp[3];
        pTemp[3] = byte;
        byte = pTemp[2];
        pTemp[2] = pTemp[1];
        pTemp[1] = byte;
    }
    *pVal = val;
    pMsg += sizeof(uint32_t);

    return pMsg;
}

inline const uint8_t* ReadUINT16(const uint8_t* pMsg, uint16_t* pVal)
{
    uint16_t val = *(uint16_t*)pMsg;

    if (O32_HOST_ORDER == O32_BIG_ENDIAN) {
        uint8_t* pTemp = (uint8_t*)&val;
        uint8_t  byte = 0;
        byte = pTemp[0];
        pTemp[0] = pTemp[1];
        pTemp[1] = byte;
    }
    *pVal = val;
    pMsg += sizeof(uint16_t);

    return pMsg;
}

inline const uint8_t* ReadUINT8(const uint8_t* pMsg, uint8_t* pVal)
{
    uint8_t val = *(uint8_t*)pMsg;
    *pVal = val;
    pMsg += sizeof(uint8_t);

    return pMsg;
}

TradeMessage::TradeMessage(const uint8_t* pHeader, uint32_t uLen) :
    marker(0),
    msg_type(EOrderEntry),
    sequence_id(0),
    timestamp(0),
    msg_direction(EToExchange)
{
    SetHeader(pHeader, uLen);
}

TradeMessage::TradeMessage(const TradeMessage& obj) :
    marker(obj.marker),
    msg_type(obj.msg_type),
    sequence_id(obj.sequence_id),
    timestamp(obj.timestamp),
    msg_direction(obj.msg_direction)
{
}

TradeMessage& TradeMessage::operator=(const TradeMessage& obj)
{
    if (this != &obj) {
        marker = obj.marker;
        msg_type = obj.msg_type;
        sequence_id = obj.sequence_id;
        timestamp = obj.timestamp;
        msg_direction = obj.msg_direction;
    }
    return *this;
}

ostream& TradeMessage::operator<<(ostream& os)
{
    DisplayHeader();
    Display();
    return os;
}

bool TradeMessage::SetHeader(const uint8_t* pHeader, uint32_t uLen)
{
    bool success = false;
    uint8_t val = 0;

    if (uLen == TRADE_HEADER_LEN) {
        success = true;
        pHeader = ReadUINT16(pHeader, &marker);
        pHeader = ReadUINT8(pHeader, &val);
        if (val == EOrderEntry || val == EOrderACK || val == EOrderFill) {
            msg_type = (MsgType)val;
        } else {
            success = false;
        }

        pHeader = ReadUINT64(pHeader, &sequence_id);
        pHeader = ReadUINT64(pHeader, &timestamp);
        pHeader = ReadUINT8(pHeader, &val);
        if (val == EToExchange || val == EFromExchange) {
            msg_direction = (MsgDirection)val;
        } else {
            success = false;
        }
        pHeader = ReadUINT16(pHeader, &msg_length);
    }

    return success;
}

bool TradeMessage::SetMessage(const uint8_t* pMessage, uint32_t uLen)
{
    bool     success = false;
    uint32_t uMsgLen = uLen - TRADE_TERM_STRINGLEN;
    //Look for the termination string before doing anything
    const uint8_t* pTerm = (const uint8_t*)pMessage;
    pTerm += uMsgLen;
    if (memcmp(pTerm, gTermString, TRADE_TERM_STRINGLEN) == 0) {
       success = SetMessage_(pMessage, uMsgLen);
    } else {
        cout<<"No Termination String!!! Ignoring this Trade Record"<<endl;
    }

    return success;
}

bool TradeMessage::SetMessage_(const uint8_t* pMessage, uint32_t uLen)
{
    cout<<"Error!! We shouldn't be reaching here"<<endl;
    return false;
}

void TradeMessage::DisplayHeader() const
{
    cout<<"=============HEADER==============="<<endl;
    cout<<"marker:      "<<marker<<endl;
    cout<<"Msg Type:    "<<+msg_type<<endl;
    cout<<"Sequence ID: "<<sequence_id<<endl;
    cout<<"Time-stamp:  "<<timestamp<<endl;
    cout<<"Msg Dir:     "<<+msg_direction<<endl;
    cout<<"Msg Len:     "<<msg_length<<endl;
    cout<<"=================================="<<endl;
}

void TradeMessage::Display() const
{
}

EntryMessage::EntryMessage(const uint8_t* pHeader, uint32_t uLen) :
    TradeMessage(pHeader, uLen),
    price(0),
    qty(0),
    side(EBUY),
    client_id(0),
    time_in_force(EIOC),
    firm_id(0),
    firm()
{
    instrument[0] = '\0';
    trader_tag[0] = '\0';
}

EntryMessage::EntryMessage(const EntryMessage& obj) :
    TradeMessage(obj),
    price(obj.price),
    qty(obj.qty),
    side(obj.side),
    client_id(obj.client_id),
    time_in_force(obj.time_in_force),
    firm_id(obj.firm_id),
    firm(obj.firm)
{
    strcpy(instrument, obj.instrument);
    strcpy(trader_tag, obj.trader_tag);
}

EntryMessage& EntryMessage::operator=(const EntryMessage& obj)
{
    if (this != &obj) {
        price = obj.price;
        qty = obj.qty;
        side = obj.side;
        client_id = obj.client_id;
        time_in_force = obj.time_in_force;
        firm_id = obj.firm_id;
        firm = obj.firm;
        strcpy(instrument, obj.instrument);
        strcpy(trader_tag, obj.trader_tag);
    }
    return *this;
}

bool EntryMessage::SetMessage_(const uint8_t* pMessage, uint32_t uLen)
{
    bool success = false;
    const uint8_t* pBase = pMessage;
    uint32_t uFirmLen = 0;
    uint8_t val = 0;

    if (uLen >= TRADE_ENTRY_MSG_MIN_LEN) {
        success = true;
        pMessage = ReadUINT64(pMessage, &price);
        pMessage = ReadUINT32(pMessage, &qty);
        memcpy(instrument, pMessage, 10);
        instrument[10] = '\0'; //NULL terminate the string
        pMessage += 10;

        pMessage = ReadUINT8(pMessage, &val);
        if (val == EBUY || val == ESELL) {
            side = (SideType)val;
        } else {
            success = false;
        }

        pMessage = ReadUINT64(pMessage, &client_id);
        pMessage = ReadUINT8(pMessage, &val);
        if (val == EIOC || val == EGFD) {
            time_in_force = (TimeInForce)val;
        } else {
            success = false;
        }

        memcpy(trader_tag, pMessage, 3);
        trader_tag[3] = '\0'; //NULL terminate the string
        pMessage += 3;

        pMessage = ReadUINT8(pMessage, &firm_id);

        //Construct the firm string now - cant be more than 254
        uFirmLen = uLen - (pMessage - pBase);
        if (uFirmLen <= 254) {
            firm.copy((char*)pMessage, uFirmLen);
        } else {
            success = false;
        }
    }

    return success;
}

void EntryMessage::Display() const
{
    cout<<"==========Entry Message============"<<endl;
    cout<<"Price:      "<<price<<endl;
    cout<<"Qty:        "<<qty<<endl;
    cout<<"Instrument: "<<instrument<<endl;
    cout<<"Side:       "<<+side<<endl;
    cout<<"Client ID:  "<<client_id<<endl;
    cout<<"TIF:        "<<+time_in_force<<endl;
    cout<<"Trade Tag:  "<<trader_tag<<endl;
    cout<<"Firm ID:    "<<+firm_id<<endl;
    cout<<"Firm:       "<<firm<<endl;
    cout<<"=================================="<<endl;
}

AckMessage::AckMessage(const uint8_t* pHeader, uint32_t uLen) :
    TradeMessage(pHeader, uLen),
    order_id(0),
    client_id(0),
    order_status(EGOOD),
    reject_code(ENoCode)
{
}

AckMessage::AckMessage(const AckMessage& obj) :
    TradeMessage(obj),
    order_id(obj.order_id),
    client_id(obj.client_id),
    order_status(obj.order_status),
    reject_code(obj.reject_code)
{
}

AckMessage& AckMessage::operator=(const AckMessage& obj)
{
    if (this != &obj) {
        order_id = obj.order_id;
        client_id = obj.client_id;
        order_status = obj.order_status;
        reject_code = obj.reject_code;
    }
    return *this;
}

bool AckMessage::SetMessage_(const uint8_t* pMessage, uint32_t uLen)
{
    bool success = false;
    uint8_t val = 0;

    if (uLen == TRADE_ACK_MSG_LEN) {
        success = true;
        pMessage = ReadUINT32(pMessage, &order_id);
        pMessage = ReadUINT64(pMessage, &client_id);

        pMessage = ReadUINT8(pMessage, &val);
        if (val == EGOOD || val == EREJECT) {
            order_status = (OrderStatus)val;
        } else {
            success = false;
        }

        pMessage = ReadUINT8(pMessage, &val);
        if (val <= EInvalidQty) {
            reject_code = (RejectCode)val;
        } else {
            success = false;
        }
    }

    return success;
}

void AckMessage::Display() const
{
    cout<<"============Ack Message============"<<endl;
    cout<<"Order ID:      "<<order_id<<endl;
    cout<<"Client ID:     "<<client_id<<endl;
    cout<<"Order Status:  "<<order_status<<endl;
    cout<<"Reject Code:   "<<reject_code<<endl;
    cout<<"=================================="<<endl;
}

#define DEFAULT_CONTRAS_COUNT 20

FillMessage::FillMessage(const uint8_t* pHeader, uint32_t uLen) :
    TradeMessage(pHeader, uLen),
    order_id(0),
    fill_price(0),
    fill_qty(0),
    no_of_contras(0)
{
    firmInfo.reserve(DEFAULT_CONTRAS_COUNT);
}

FillMessage::FillMessage(const FillMessage& obj) :
    TradeMessage(obj),
    order_id(obj.order_id),
    fill_price(obj.fill_price),
    fill_qty(obj.fill_qty),
    no_of_contras(obj.no_of_contras),
    firmInfo(obj.firmInfo)
{
}

FillMessage& FillMessage::operator=(const FillMessage& obj)
{
    if (this != &obj) {
        order_id = obj.order_id;
        fill_price = obj.fill_price;
        fill_qty = obj.fill_qty;
        no_of_contras = obj.no_of_contras;
        firmInfo = obj.firmInfo;
    }
    return *this;
}

bool FillMessage::SetMessage_(const uint8_t* pMessage, uint32_t uLen)
{
    bool success = false;
    FirmInfo fInfo;

    if (uLen >= TRADE_FILL_MSG_MIN_LEN) {
        success = true;
        pMessage = ReadUINT32(pMessage, &order_id);
        pMessage = ReadUINT64(pMessage, &fill_price);
        pMessage = ReadUINT32(pMessage, &fill_qty);
        pMessage = ReadUINT8(pMessage, &no_of_contras);

        //Copy the Contras of "no_of_contras" count
        for (int index = 0; index < no_of_contras; index++) {
            pMessage = ReadUINT8(pMessage, &fInfo.firm_id);
            memcpy(fInfo.trader_tag, pMessage, 3);
            fInfo.trader_tag[3] = '\0'; //NULL terminate the string
            pMessage += 3;
            pMessage = ReadUINT32(pMessage, &fInfo.qty);
            firmInfo.push_back(fInfo);
        }
    }

    return success;
}

void FillMessage::Display() const
{
    cout<<"===========Fill Message============"<<endl;
    cout<<"Order ID:      "<<order_id<<endl;
    cout<<"Fill Price:    "<<fill_price<<endl;
    cout<<"Fill Qty:      "<<fill_qty<<endl;
    cout<<"No of Contras: "<<+no_of_contras<<endl;
    for (int i=0; i<no_of_contras; i++) {
        cout<<"Firm ID:       "<<+firmInfo[i].firm_id<<endl;
        cout<<"Trader TAG:    "<<firmInfo[i].trader_tag<<endl;
        cout<<"Qty:           "<<firmInfo[i].qty<<endl;
    }
    cout<<"=================================="<<endl;
}


#define DEFAULT_RESERVE_SIZE 1000

TradeMessageList::TradeMessageList()
{
    iOrderEntryList.reserve(DEFAULT_RESERVE_SIZE);
    iOrderAckList.reserve(DEFAULT_RESERVE_SIZE);
    iOrderFillList.reserve(DEFAULT_RESERVE_SIZE);
}

TradeMessageList::~TradeMessageList()
{
    uint32_t index = 0;
    uint32_t totalRecords = iOrderEntryList.size();
    for (index = 0; index < totalRecords; index++) {
       delete iOrderEntryList[index];
    }
    iOrderEntryList.clear();

    totalRecords = iOrderAckList.size();
    for (index = 0; index < totalRecords; index++) {
       delete iOrderAckList[index];
    }
    iOrderAckList.clear();

    totalRecords = iOrderFillList.size();
    for (index = 0; index < totalRecords; index++) {
       delete iOrderFillList[index];
    }
    iOrderFillList.clear();
}

//Adds one Trade transaction record to the List
void TradeMessageList::Add(TradeMessage* pMsg)
{
    MsgType msgType = pMsg->MessageType();
    switch (msgType) {
    case EOrderEntry:
        iOrderEntryList.push_back((EntryMessage*)pMsg);
        break;
    case EOrderACK:
        iOrderAckList.push_back((AckMessage*)pMsg);
        break;
    case EOrderFill:
        iOrderFillList.push_back((FillMessage*)pMsg);
        break;
    default:
        cout<<"Error: Ignore this!!"<<endl;
    }
}

//Gets the count of all Trade transaction records
uint32_t TradeMessageList::GetAllRecordCount() const
{
    uint32_t sum = iOrderEntryList.size() + iOrderAckList.size() + iOrderFillList.size();

    return sum;
}

//Gets the count of individual Trade transaction records
uint32_t TradeMessageList::GetTradeRecordsCount(MsgType aType) const
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
bool CompareOrderEntry(const EntryMessage* lhs, const EntryMessage* rhs) {
    return lhs->client_id < rhs->client_id;
}

//Comparison function for Order ACK records based on Client ID
bool CompareOrderAck(const AckMessage* lhs, const AckMessage* rhs) {
    return lhs->client_id < rhs->client_id;
}

//Comparison function for Order Fill records based on Quantity of Trade
bool CompareOrderFill(const FillMessage* lhs, const FillMessage* rhs) {
    return lhs->fill_qty > rhs->fill_qty ;
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
        if (client_id != iOrderAckList[index]->client_id) {
            client_id = iOrderAckList[index]->client_id;
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
        order_id = iOrderAckList[index]->order_id;
        fill_qty = 0;

        //Using the Trade Order Fill records, get the trade quantity for each order
        for(index_= 0; index_ < totalSalesRecords; index_++){
            order_id_ = iOrderFillList[index_]->order_id;

            if(order_id == order_id_) {
                fill_qty = iOrderFillList[index_]->fill_qty;
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
        if (maxTrade.client_id == iOrderEntryList[index]->client_id) {
            pActiveTrader = iOrderEntryList[index]->trader_tag;
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
        ttif = iOrderEntryList[index]->time_in_force;
        if (client_id != iOrderEntryList[index]->client_id && ttif == EGFD) {
            client_id = iOrderEntryList[index]->client_id;
            clientOrderList.trader_tag =  iOrderEntryList[index]->trader_tag;
            clientOrderList.total_liquidity =
              iOrderEntryList[index]->qty * iOrderEntryList[index]->price;
            if (maxClientOrderList.total_liquidity < clientOrderList.total_liquidity) {
                maxClientOrderList = clientOrderList;
            }
        } else if (ttif == EGFD) {
            clientOrderList.total_liquidity +=
              iOrderEntryList[index]->qty * iOrderEntryList[index]->price;
            //See whether this client has the max trade quantity
            if (maxClientOrderList.total_liquidity < clientOrderList.total_liquidity) {
                maxClientOrderList = clientOrderList;
            }
        }
    }
    return maxClientOrderList.trader_tag;
}

struct InstrumentOrderList {
    uint32_t qty;
    uint64_t client_id;
    uint32_t order_id;

    InstrumentOrderList() : qty(0),client_id(0),order_id(0){}
};

struct Instruments {
    const char* instrument;
    uint32_t qty;
    Instruments(): instrument(NULL),qty(0){}
};

void TradeMessageList::GetTradesPerInstrument()
{
    uint32_t totalEntry = iOrderEntryList.size();
    uint32_t totalFill = iOrderFillList.size();
    uint32_t totalAck = iOrderAckList.size();
    vector<Instruments> tempList;
    Instruments         tempObj;
    uint32_t index = 0;
    uint32_t index_ = 0;
    uint32_t count = 0;
    uint64_t client_id;
    const char* pTemp = NULL;
    const char* pTemp_ = NULL;

    InstrumentOrderList* instOrderList = new InstrumentOrderList[totalFill];
    for (; index < totalFill; index++){
        instOrderList[index].order_id = iOrderFillList[index]->order_id;
        instOrderList[index].qty = iOrderFillList[index]->fill_qty;
    }
    for(index = 0; index < totalAck; index++) {
        uint32_t order_id = iOrderAckList[index]->order_id;
        OrderStatus status = iOrderAckList[index]->order_status;
        if(status == EGOOD) {
            for(index_ = 0; index_ < totalFill; index_++){
                if(order_id == instOrderList[index_].order_id ){
                    instOrderList[index_].client_id = iOrderAckList[index]->client_id;
                    break;
                }
            }
        }
    }

    for (index_ = 0; index_ < totalFill; index_++) {
        client_id = instOrderList[index_].client_id;
        if (client_id != 0) {
            for(index=0;index<totalEntry;index++) {
                if(client_id == iOrderEntryList[index]->client_id) {
                    tempObj.instrument = iOrderEntryList[index]->instrument;
                    tempObj.qty = instOrderList[index_].qty;
                    tempList.push_back(tempObj);
                }
            }
        }
    }

    delete instOrderList;
    count = tempList.size();
    for(index = 0; index < count; index++) {
        pTemp = tempList[index].instrument;
        if (pTemp) {
            for (index_ = index + 1; index_ < count; index_++) {
                pTemp_ = tempList[index_].instrument;
                if (pTemp_ && strcmp(pTemp, pTemp_) == 0) {
                    tempList[index].qty += tempList[index_].qty;
                    //We have already consumed it
                    tempList[index_].instrument = NULL;
                }
            }
        }
    }

    for(index = 0; index < count; index++) {
        pTemp = tempList[index].instrument;
        if (pTemp) {
            cout<<", "<<pTemp<<" : "<<tempList[index].qty;
        }
    }
}

