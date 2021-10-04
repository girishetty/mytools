#ifndef TRADEMESSAGE_H_INCLUDED
#define TRADEMESSAGE_H_INCLUDED

#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//#define _DEBUG

#define TRADE_HEADER_LEN          22

#ifdef _DEBUG
void hex_dump(const char* name, const uint8_t* data, const int uLen);
#else
#define hex_dump(...)
#endif // _DEBUG

//Trade Message Types
enum MsgType {
    EOrderEntry = 1,
    EOrderACK,
    EOrderFill,
};

//Trade Message Direction
enum MsgDirection {
    EToExchange = 0,
    EFromExchange,
};

//Trade Time In Force type
enum TimeInForce {
    EIOC = 1,
    EGFD,
};

//Trade Side type
enum SideType {
    EBUY = 1,
    ESELL,
};

//Trade Order Status
enum OrderStatus {
    EGOOD = 1,
    EREJECT,
};

//Trade Reject Code
enum RejectCode {
    ENoCode = 0,
    EInvalidInput,
    EInvalidPrice,
    EInvalidQty,
};

struct TradeMessageList;

struct TradeMessage {

public:
    TradeMessage(const uint8_t* pHeader, uint32_t uLen);
    TradeMessage(const TradeMessage& obj);
    static TradeMessage* CreateInstance(const uint8_t* pHeader, uint32_t uLen);
    TradeMessage& operator=(const TradeMessage& obj);
    ostream& operator<<(ostream& os);
    bool SetHeader(const uint8_t* pHeader, uint32_t uLen);
    bool SetMessage(const uint8_t* pMessage, uint32_t uLen);
    inline uint16_t MessageLength() const { return msg_length; };
    inline MsgType MessageType() const { return msg_type; };
    void DisplayHeader() const;

    virtual ~TradeMessage() {};
    virtual void Display() const;

    friend struct TradeMessageList;

private:
    virtual bool SetMessage_(const uint8_t* pMessage, uint32_t uLen);

protected:
    uint16_t     marker;
    MsgType      msg_type;
    uint64_t     sequence_id;
    uint64_t     timestamp;
    MsgDirection msg_direction;
    uint16_t     msg_length;
};

struct EntryMessage : public TradeMessage {

public:
    EntryMessage(const uint8_t* pHeader, uint32_t uLen);
    EntryMessage(const EntryMessage& obj);
    EntryMessage& operator=(const EntryMessage& obj);

    virtual ~EntryMessage() { };
    virtual void Display() const;
    virtual bool SetMessage_(const uint8_t* pMessage, uint32_t uLen);

    friend struct TradeMessageList;
    friend bool CompareOrderEntry(const EntryMessage* lhs, const EntryMessage* rhs);

protected:
    uint64_t    price;
    uint32_t    qty;
    char        instrument[11];
    SideType    side;
    uint64_t    client_id;
    TimeInForce time_in_force;
    char        trader_tag[4];
    uint8_t     firm_id;
    string      firm;
};

struct AckMessage : public TradeMessage {

public:
    AckMessage(const uint8_t* pHeader, uint32_t uLen);
    AckMessage(const AckMessage& obj);
    AckMessage& operator=(const AckMessage& obj);

    virtual ~AckMessage() { };
    virtual void Display() const;
    virtual bool SetMessage_(const uint8_t* pMessage, uint32_t uLen);

    friend struct TradeMessageList;
    friend bool CompareOrderAck(const AckMessage* lhs, const AckMessage* rhs);

protected:
    uint32_t    order_id;
    uint64_t    client_id;
    OrderStatus order_status;
    RejectCode  reject_code;
};

struct FirmInfo {
   uint8_t  firm_id;
   char     trader_tag[4];
   uint32_t qty;
};

struct FillMessage : public TradeMessage {

public:
    FillMessage(const uint8_t* pHeader, uint32_t uLen);
    FillMessage(const FillMessage& obj);
    FillMessage& operator=(const FillMessage& obj);

    virtual ~FillMessage() { };
    virtual void Display() const;
    virtual bool SetMessage_(const uint8_t* pMessage, uint32_t uLen);

    friend struct TradeMessageList;
    friend bool CompareOrderFill(const FillMessage* lhs, const FillMessage* rhs);

protected:
    uint32_t         order_id;
    uint64_t         fill_price;
    uint32_t         fill_qty;
    uint8_t          no_of_contras;
    vector<FirmInfo> firmInfo;
};

struct TradeMessageList {
public:
    TradeMessageList();
    ~TradeMessageList();

    void Add(TradeMessage* pMsg);
    uint32_t GetAllRecordCount() const;
    uint32_t GetTradeRecordsCount(MsgType aType) const;
    const char* GetActiveTrader();
    const char* GetHighestLiquidTrader();
    void GetTradesPerInstrument();

private:
    vector<EntryMessage*> iOrderEntryList;
    vector<AckMessage*>   iOrderAckList;
    vector<FillMessage*>  iOrderFillList;
};

#endif // TRADEMESSAGE_H_INCLUDED
