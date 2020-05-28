#ifndef TRADEMESSAGE_H_INCLUDED
#define TRADEMESSAGE_H_INCLUDED

#include <stdint.h>
#include <vector>

using namespace std;

//#define _DEBUG

#define LEN_TERM_STRING 8
extern const char* const gTermString;

#ifdef _DEBUG
void hex_dump(const char* name, const uint8_t* data, const int uLen);
#endif // _DEBUG

enum TradeMessageType {
    EOrderEntry = 1,
    EOrderACK,
    EOrderFill,
};

enum TradeTimeInForce {
    EIOC = 1,
    EGFD,
};

#pragma pack(push, 1)

struct TradeHeader {
    #if 0
    TradeHeader() {}
    TradeHeader(const TradeHeader& th) {
        marker = th.marker;
    }
    #endif // 0
    uint16_t marker; //equal to ST
    uint8_t msg_type;// orderEntry=1;orderAck=2;orderFill=3
    uint64_t sequence_id;
    uint64_t timestamp; //seconds
    uint8_t msg_direction; //0 to exchange ;1 from exchange
    uint16_t msg_length;

#ifdef _DEBUG
    void Display() const;
#endif // DEBUG
};

struct EntryMSG {
    uint64_t price; //divide by 10000
    uint32_t qty;
    char instrument[10];
    uint8_t side; //buy=1;sell=2
    uint64_t client_assigned_id;
    uint8_t time_in_force;//IOC=1;GFD=2
    char trader_tag[3];
    uint8_t firm_id;
    char firm[256];
#ifdef _DEBUG
    void Display() const;
#endif // DEBUG
};

struct AckMSG {
    uint32_t order_id;
    uint64_t client_id;
    uint8_t order_status;//good=1;reject=2
    uint8_t reject_code;//no code=0;invalidInput=1;invalidPrice=2;invalidQty=3
};

struct FirmInfo {
   uint8_t firm_id;
   char trader_tag[3];
   uint32_t qty;
};

#define MAX_FIRM_INFO 20

struct FillMSG {
    uint32_t order_id;
    uint64_t fill_price;//divide by 10000
    uint32_t fill_qty;
    uint8_t no_of_contras;
    FirmInfo firmInfo[MAX_FIRM_INFO];
#ifdef _DEBUG
    void Display() const;
#endif // _DEBUG
};

struct TradeMessage {
#if 0
    TradeMessage() {}
    TradeMessage(const TradeMessage& tm) : iHeader(tm.iHeader) {
        switch(iHeader.msg_type) {
        case EOrderEntry:
            iMessage.iEntry = tm.iMessage.iEntry;
            break;
        case EOrderACK:
            iMessage.iAck = tm.iMessage.iAck;
            break;
        case EOrderFill:
            iMessage.iFill = tm.iMessage.iFill;
            break;
        }
    }
#endif // 0
    TradeHeader iHeader;
    union Message {
        char     iMsg[sizeof(EntryMSG) + LEN_TERM_STRING];
        EntryMSG iEntry;
        AckMSG   iAck;
        FillMSG  iFill;
    } iMessage;
};

#define DEFAULT_RESERVE_SIZE 2000

struct TradeMessageList {
public:
    #if 0
    TradeMessageList() {
        //iOrderEntryList.reserve(DEFAULT_RESERVE_SIZE);
        //iOrderAckList.reserve(DEFAULT_RESERVE_SIZE);
        //iOrderFillList.reserve(DEFAULT_RESERVE_SIZE);
    }
    #endif // 0
    void Add(TradeMessage* pMsg, uint32_t uLen);
    uint32_t GetAllRecordCount() const;
    uint32_t GetTradeRecordsCount(TradeMessageType aType) const;
    const char* GetActiveTrader();
    const char* GetHighestLiquidTrader();

private:
    //vector<TradeMessage> iDummyAckList;
    vector<TradeMessage> iOrderAckList;
    vector<TradeMessage> iOrderFillList;
    vector<TradeMessage> iOrderEntryList;
};

#endif // TRADEMESSAGE_H_INCLUDED
