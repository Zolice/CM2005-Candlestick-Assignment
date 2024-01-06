// Start of Original Code without Assistance

#pragma once

#include <string>
#include <vector>
#include <set>

#include "Candlestick.h"

class CandlestickBook
{
public:
    CandlestickBook(std::string filename);

    /** return vector of all know products in the dataset*/
    std::vector<std::string> getKnownProducts();

    /** returns the earliest time in the orderbook*/
    std::string getEarliestTime();

    int getTotalTimestampCount();

    void ReduceCandlesticks(int count);

    std::vector<Candlestick> candlesticks;
    int totalTimestampCount;
    std::set<std::string> allTimestamps;

private:
};

// End of Original Code without Assistance