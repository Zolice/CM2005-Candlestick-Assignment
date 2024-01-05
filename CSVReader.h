#pragma once

#include "OrderBookEntry.h"
#include "Candlestick.h"
#include <vector>
#include <string>

class CSVReader
{
public:
    CSVReader();

    static std::vector<OrderBookEntry> readCSV(std::string csvFile);

    // Start of Original Code without Assistance
    static std::vector<Candlestick> readCSV(std::string csvFilename, std::string product, OrderBookType orderType);
    // End of Original Code without Assistance

    static std::vector<std::string> tokenise(std::string csvLine, char separator);

    static OrderBookEntry stringsToOBE(std::string price,
                                       std::string amount,
                                       std::string timestamp,
                                       std::string product,
                                       OrderBookType OrderBookType);

    // Start of Original Code without Assistance
    static Candlestick stringsToCS(std::vector<std::string> tokens, std::string product, OrderBookType orderType);
    // End of Original Code without Assistance

private:
    static OrderBookEntry stringsToOBE(std::vector<std::string> strings);
};