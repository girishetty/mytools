#ifndef TRADEANALIZER_H_INCLUDED
#define TRADEANALIZER_H_INCLUDED

#include "TradeMessage.h"
#include "FileReader.h"

class TradeAnalyzer {
public:
    TradeAnalyzer(const char* pInput) : iInput(pInput) {};
    int StartProcessing();
    void GetStatistics();
    inline void SetInput(const char* pInput) {
        iInput.Open(pInput);
    }

private:
    StreamReader     iInput;
    TradeMessageList iList;
};

#endif // TRADEANALUZER_H_INCLUDED
