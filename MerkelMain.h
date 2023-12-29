#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "Candlestick.h"
#include "CandlestickBook.h"

class MerkelMain
{
public:
    MerkelMain();
    /** Call this to start the sim */
    void init();

private:
    void printMenu();
    void printHelp();
    void printMarketStats();
    void enterAsk();
    void enterBid();
    void printWallet();
    void gotoNextTimeframe();
    int getUserOption();
    void computeCandlesticks(int candlesticksPerProduct);
    void computeCustomCandlesticks(); // int candlesticksPerProduct
    void printProgress(int progress, int total, bool firsIteration);
    void requestCandlesticks();
    void requestCustomCandlesticks();
    void drawCandlesticks(std::vector<Candlestick> candlesticks);
    void processUserOption(int userOption);

    std::string currentTime;

    OrderBook orderBook{"20200317.csv"};
    // OrderBook orderBook{"20200601.csv"};
    CandlestickBook candlestickBook{"WEOS-USD.csv"};
    Wallet wallet;

    std::vector<std::vector<Candlestick>> candlesticks;
    std::vector<Candlestick> customCandlesticks;
};
