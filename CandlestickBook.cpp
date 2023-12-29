#include "CandlestickBook.h"
#include "CSVReader.h"
#include <map>
#include <iostream>
#include <cmath>

CandlestickBook::CandlestickBook(std::string filename)
{
    candlesticks = CSVReader::readCSV(filename, "WEOS/USD", OrderBookType::transaction);
    std::cout << "Hello";

    totalTimestampCount = getTotalTimestampCount();
}

std::vector<std::string> CandlestickBook::getKnownProducts()
{
    std::vector<std::string> products;

    std::map<std::string, bool> prodMap;

    for (Candlestick &e : candlesticks)
    {
        prodMap[e.product] = true;
    }

    // now flatten the map to a vector of strings
    for (auto const &e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}

std::string CandlestickBook::getEarliestTime()
{
    return candlesticks[0].timestamp;
}

int CandlestickBook::getTotalTimestampCount()
{
    std::cout << "Hello";
    std::set<std::string> timestamps;
    for (Candlestick &e : candlesticks)
    {
        timestamps.insert(e.timestamp);
    }

    allTimestamps = timestamps;
    return timestamps.size();
}

/**
 * @brief Reduce the number of candlesticks to the count specified. 
 * This function will combine candlesticks together to achieve the desired outcome. 
 * 
 * @param count The number of candlesticks to reduce to.
 */
void CandlestickBook::ReduceCandlesticks(int count) {
    if(count >= candlesticks.size())
        return; // If the count is greater than the number of candlesticks, return.
    
    // Get the number of candlesticks to merge into one. 
    int mergeCount = ceil(totalTimestampCount / count);

    // Create a new vector to store the new merged candlesticks
    std::vector<Candlestick> csb;

    // For each candlestick, merge the next mergeCount candlesticks into it.
    for (int i = 0; i < candlesticks.size(); i+= mergeCount) {
        // Get the data from the candlesticks to merge. 
        // Declare variables to store the data. 
        double high = candlesticks[i].high;
        double low = candlesticks[i].low;
        double open = candlesticks[i].open;
        
        // These 2 variables will be used for calculation. 
        double volume = candlesticks[i].volume;
        double totalCost = candlesticks[i].volume * candlesticks[i].close;

        // For each available candlesticks to merge within the range, add the data to the variables.
        for (int j = i; j < i + mergeCount; j++) {
            if (j >= candlesticks.size())
                break; // If the index is out of range, break. 
            if (candlesticks[j].high > high)
                high = candlesticks[j].high;
            if (candlesticks[j].low < low)
                low = candlesticks[j].low;
            volume += candlesticks[j].volume;
            totalCost += candlesticks[j].volume * candlesticks[j].close;
        }

        // Calculate the close price
        double close = totalCost / volume;

        // Create a new candlestick with the new data.
        Candlestick cs;
        cs.setValues(open, close, high, low, volume, candlesticks[i].timestamp, candlesticks[i].product, candlesticks[i].orderType);

        // Add the new candlestick to the vector.
        csb.push_back(cs);
    }

    // Overwrite the old candlesticks with the new candlesticks.
    candlesticks = csb;
}