#include <vector>
#include <iostream>

#include "Candlestick.h"

Candlestick::Candlestick()
{
}

Candlestick::Candlestick(std::vector<OrderBookEntry> &orders)
{
    AddOrders(orders);
}

void Candlestick::AddOrders(std::vector<OrderBookEntry> &orders)
{
    if (orders.empty())
        return; // if there are no orders, return

    // Add all orders to _orders
    for (OrderBookEntry &e : orders)
    {
        _orders.push_back(e);
    }

    // Set other Information
    Timestamp = orders[0].timestamp;
    Product = orders[0].product;
    OrderType = orders[0].orderType;

    // Calculate the Open, Close, High, and Low prices
    // Open = computeOpenPrice();
    Close = computeClosePrice();
    High = computeHighPrice();
    Low = computeLowPrice();
}

/**
 * @brief Prints the Candlestick's information to the console.
 * 
 */
void Candlestick::PrintInformation()
{
    std::cout << "Timestamp: " << Timestamp << std::endl;
    std::cout << "Product: " << Product << std::endl;
    // std::cout << "OrderType: " << OrderType << std::endl;
    std::cout << "Open: " << Open << std::endl;
    std::cout << "Close: " << Close << std::endl;
    std::cout << "High: " << High << std::endl;
    std::cout << "Low: " << Low << std::endl;
}

/**
 * @brief Sets the Open price of the candlestick.
 * Call this if Open price needs to be set manually.
 *
 * @param openPrice
 */
void Candlestick::SetOpenPrice(double openPrice)
{
    Open = openPrice;
}

/**
 * @brief Calculates the average price per unit in the previous time frame.
 * Call this if Open price needs to be recalculated.
 *
 * @return Open Price
 */
double Candlestick::computeOpenPrice()
{
    return 0.0;
}

/**
 * @brief Calculates the average price per unit in the current time frame.
 * Call this if Close price needs to be recalculated.
 *
 * @return Close Price
 */
double Candlestick::computeClosePrice()
{
    if (_orders.empty())
        return 0.0; // if there are no orders, return 0.0

    // Get the total price and count
    double totalPrice = 0.0;
    int count = 0;

    for (OrderBookEntry &e : _orders)
    {
        totalPrice += e.price;
        count++;
    }

    // Store the average price per unit in Close
    Close = totalPrice / count;

    // Return Close value
    return Close;
}

/**
 * @brief Calculates the Highest price seen this time frame
 * Call this if High price needs to be recalculated.
 *
 * @return High Price
 */
double Candlestick::computeHighPrice()
{
    if (_orders.empty())
        return 0.0; // if there are no orders, return 0.0

    // Get the highest price
    double highestPrice = _orders[0].price;

    for (OrderBookEntry &e : _orders)
    {
        if (e.price > highestPrice)
            highestPrice = e.price;
    }

    // Store the highest price in High
    High = highestPrice;

    // Return High value
    return High;
}

/**
 * @brief Calculates the Lowest price seen this time frame
 * Call this if Low price needs to be recalculated.
 *
 * @return Low Price
 */
double Candlestick::computeLowPrice()
{
    if (_orders.empty())
        return 0.0; // if there are no orders, return 0.0

    // Get the lowest price
    double lowestPrice = _orders[0].price;

    for (OrderBookEntry &e : _orders)
    {
        if (e.price < lowestPrice)
            lowestPrice = e.price;
    }

    // Store the lowest price in Low
    Low = lowestPrice;

    // Return Low value
    return Low;
}