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

    // Start of Original Code without Assistance
    void computeCandlesticks();
    void computeCustomCandlesticks(); // int candlesticksPerProduct
    void printProgress(int progress, int total, bool firsIteration);
    void requestCandlesticks();
    void requestCustomCandlesticks();
    void drawCandlesticks(std::vector<Candlestick> candlesticks);
    void writeToFile(const std::vector<std::string> &data, const std::string &filename);
    // End of Original Code without Assistance

    void processUserOption(int userOption);

    std::string currentTime;

    // OrderBook orderBook{"20200317.csv"};
    OrderBook orderBook{"20200601.csv"};

    // Start of Original Code without Assistance
    std::vector<std::vector<Candlestick>> candlesticks;
    std::vector<Candlestick> backup;
    CandlestickBook candlestickBook{"WEOS-USD.csv"};
    // End of Original Code without Assistance

    Wallet wallet;
};