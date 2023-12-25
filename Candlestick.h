#pragma once

#include <vector>
#include <string>

#include "OrderBookEntry.h"

class Candlestick
{
public:
    std::string timestamp;
    std::string product;
    OrderBookType orderType;

    double open;
    double close;
    double high;
    double low;

    Candlestick();

    Candlestick(std::vector<OrderBookEntry> &orders);

    void addOrders(std::vector<OrderBookEntry> &orders);

    void printInformation(); 

    void setOpenPrice(double openPrice);

private:
    // std::vector<OrderBookEntry> _orders;

    double computeOpenPrice(std::vector<OrderBookEntry> &orders);
    double computeClosePrice(std::vector<OrderBookEntry> &orders);
    double computeHighPrice(std::vector<OrderBookEntry> &orders);
    double computeLowPrice(std::vector<OrderBookEntry> &orders);
};