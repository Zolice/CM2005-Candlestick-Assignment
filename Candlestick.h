#pragma once

#include <vector>
#include <string>

#include "OrderBookEntry.h"

class Candlestick
{
public:
    std::string Timestamp;
    std::string Product;
    OrderBookType OrderType;

    double Open;
    double Close;
    double High;
    double Low;

    Candlestick();

    Candlestick(std::vector<OrderBookEntry> &orders);

    void AddOrders(std::vector<OrderBookEntry> &orders);

    void PrintInformation(); 

    void SetOpenPrice(double openPrice);

private:
    std::vector<OrderBookEntry> _orders;

    double computeOpenPrice();
    double computeClosePrice();
    double computeHighPrice();
    double computeLowPrice();
};