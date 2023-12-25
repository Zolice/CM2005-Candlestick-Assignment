#include <vector>
#include <iostream>

#include "Candlestick.h"

Candlestick::Candlestick()
{
}

Candlestick::Candlestick(std::vector<OrderBookEntry> &orders)
{
    addOrders(orders);
}

void Candlestick::addOrders(std::vector<OrderBookEntry> &orders)
{
    if (orders.empty())
        return; // if there are no orders, return

    // Set other Information
    timestamp = orders[0].timestamp;
    product = orders[0].product;
    orderType = orders[0].orderType;

    // Calculate the Open, Close, High, and Low prices
    // Open = computeOpenPrice();
    close = computeClosePrice(orders);
    high = computeHighPrice(orders);
    low = computeLowPrice(orders);
}

/**
 * @brief Prints the Candlestick's information to the console.
 *
 */
void Candlestick::printInformation()
{
    std::cout << "Timestamp: " << timestamp << std::endl;
    std::cout << "Product: " << product << " (" << orderBookTypeToString(orderType) << ")" << std::endl;
    // std::cout << "OrderType: " << orderBookTypeToString(orderType) << std::endl;
    std::cout << "Open: " << open << std::endl;
    std::cout << "Close: " << close << std::endl;
    std::cout << "High: " << high << std::endl;
    std::cout << "Low: " << low << std::endl;
}

/**
 * @brief Sets the Open price of the candlestick.
 * Call this if Open price needs to be set manually.
 *
 * @param openPrice
 */
void Candlestick::setOpenPrice(double openPrice)
{
    open = openPrice;
}

/**
 * @brief Calculates the average price per unit in the previous time frame.
 * Call this if Open price needs to be recalculated.
 *
 * @return Open Price
 */
double Candlestick::computeOpenPrice(std::vector<OrderBookEntry> &orders)
{
    return 0.0;
}

/**
 * @brief Calculates the average price per unit in the current time frame.
 * Call this if Close price needs to be recalculated.
 *
 * @return Close Price
 */
double Candlestick::computeClosePrice(std::vector<OrderBookEntry> &orders)
{
    if (orders.empty())
        return 0.0; // if there are no orders, return 0.0

    // Get the total price and count
    double totalPrice = 0.0;
    int count = 0;

    for (OrderBookEntry &e : orders)
    {
        totalPrice += e.price;
        count++;
    }

    // Store the average price per unit in Close
    close = totalPrice / count;

    // Return Close value
    return close;
}

/**
 * @brief Calculates the Highest price seen this time frame
 * Call this if High price needs to be recalculated.
 *
 * @return High Price
 */
double Candlestick::computeHighPrice(std::vector<OrderBookEntry> &orders)
{
    if (orders.empty())
        return 0.0; // if there are no orders, return 0.0

    // Get the highest price
    double highestPrice = orders[0].price;

    for (OrderBookEntry &e : orders)
    {
        if (e.price > highestPrice)
            highestPrice = e.price;
    }

    // Store the highest price in High
    high = highestPrice;

    // Return High value
    return high;
}

/**
 * @brief Calculates the Lowest price seen this time frame
 * Call this if Low price needs to be recalculated.
 *
 * @return Low Price
 */
double Candlestick::computeLowPrice(std::vector<OrderBookEntry> &orders)
{
    if (orders.empty())
        return 0.0; // if there are no orders, return 0.0

    // Get the lowest price
    double lowestPrice = orders[0].price;

    for (OrderBookEntry &e : orders)
    {
        if (e.price < lowestPrice)
            lowestPrice = e.price;
    }

    // Store the lowest price in Low
    low = lowestPrice;

    // Return Low value
    return low;
}