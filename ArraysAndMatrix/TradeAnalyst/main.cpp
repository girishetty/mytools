#include <iostream>
#include <fstream>
#include <string.h>
#include "TradeAnalyzer.h"
#include "TradeMessage.h"

using namespace std;

int main(int argc, char* argv[])
{
    TradeAnalyzer tAnalyzer(argv[1]);
    tAnalyzer.StartProcessing();
    tAnalyzer.GetStatistics();

    return 0;
}
