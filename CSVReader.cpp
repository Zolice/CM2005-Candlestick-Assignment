#include "CSVReader.h"
#include "Candlestick.h"
#include <iostream>
#include <fstream>

CSVReader::CSVReader()
{
}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename)
{
    std::vector<OrderBookEntry> entries;

    std::ifstream csvFile{csvFilename};
    std::string line;
    if (csvFile.is_open())
    {
        while (std::getline(csvFile, line))
        {
            try
            {
                OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
                entries.push_back(obe);
            }
            catch (const std::exception &e)
            {
                std::cout << "CSVReader::readCSV bad data" << std::endl;
            }
        } // end of while
    }

    std::cout << "CSVReader::readCSV read " << entries.size() << " entries" << std::endl;
    return entries;
}

/**
 * @brief Read a CSV file and return a vector of Candlesticks
 *
 * @param csvFilename Name of the csv file to read
 * @param product The product type
 * @param orderType The order type
 * @return std::vector<OrderBookEntry>
 */
std::vector<Candlestick> CSVReader::readCSV(std::string csvFilename, std::string product, OrderBookType orderType)
{
    std::vector<Candlestick> entries;

    std::ifstream csvFile{csvFilename};
    std::string line;
    if (csvFile.is_open())
    {
        while (std::getline(csvFile, line))
        {
            try
            {
                Candlestick cs = stringsToCS(tokenise(line, ','), product, orderType);

                entries.push_back(cs);
            }
            catch (const std::exception &e)
            {
                std::cout << "CSVReader::readCSV bad data" << std::endl;
            }
        } // end of while
    }

    std::cout << "CSVReader::readCSV read " << entries.size() << " entries" << std::endl;
    return entries;
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;
    signed int start, end;
    std::string token;
    start = csvLine.find_first_not_of(separator, 0);
    do
    {
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end)
            break;
        if (end >= 0)
            token = csvLine.substr(start, end - start);
        else
            token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
        start = end + 1;
    } while (end > 0);

    return tokens;
}

/**
 * @brief Convert a vector of strings into a Candlestick Object.
 *
 * @param tokens
 * @return OrderBookEntry
 */
Candlestick CSVReader::stringsToCS(std::vector<std::string> tokens, std::string product, OrderBookType orderType)
{
    double open, high, low, close, volume;

    if (tokens.size() != 7) // bad
    {
        std::cout << "Bad line " << std::endl;
        throw std::exception{};
    }
    // we have 7 tokens
    try
    {
        open = std::stod(tokens[1]);
        high = std::stod(tokens[2]);
        low = std::stod(tokens[3]);
        close = std::stod(tokens[4]);
        volume = std::stod(tokens[6]);
    }
    catch (const std::exception &e)
    {
        std::cout << "CSVReader::stringsToCS Bad float! " << tokens[1] << std::endl;
        std::cout << "CSVReader::stringsToCS Bad float! " << tokens[2] << std::endl;
        std::cout << "CSVReader::stringsToCS Bad float! " << tokens[3] << std::endl;
        std::cout << "CSVReader::stringsToCS Bad float! " << tokens[4] << std::endl;
        throw;
    }
    Candlestick cs;
    cs.setValues(open, close, high, low, volume, tokens[4], product, orderType);

    return cs;
}

OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens)
{
    double price, amount;

    if (tokens.size() != 5) // bad
    {
        std::cout << "Bad line " << std::endl;
        throw std::exception{};
    }
    // we have 5 tokens
    try
    {
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
    }
    catch (const std::exception &e)
    {
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[3] << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[4] << std::endl;
        throw;
    }

    OrderBookEntry obe{price,
                       amount,
                       tokens[0],
                       tokens[1],
                       OrderBookEntry::stringToOrderBookType(tokens[2])};

    return obe;
}

OrderBookEntry CSVReader::stringsToOBE(std::string priceString,
                                       std::string amountString,
                                       std::string timestamp,
                                       std::string product,
                                       OrderBookType orderType)
{
    double price, amount;
    try
    {
        price = std::stod(priceString);
        amount = std::stod(amountString);
    }
    catch (const std::exception &e)
    {
        std::cout << "CSVReader::stringsToOBE Bad float! " << priceString << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << amountString << std::endl;
        throw;
    }
    OrderBookEntry obe{price,
                       amount,
                       timestamp,
                       product,
                       orderType};

    return obe;
}