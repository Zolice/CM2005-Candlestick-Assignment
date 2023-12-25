#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "Candlestick.h"

MerkelMain::MerkelMain()
{
}

void MerkelMain::init()
{
    int input;

    wallet.insertCurrency("BTC", 10);

    while (true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}

void MerkelMain::printMenu()
{
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an offer " << std::endl;
    // 4 make a bid
    std::cout << "4: Make a bid " << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;
    // 6 continue
    std::cout << "6: Continue " << std::endl;
    // 7: compute Candlesticks using given dataset
    std::cout << "7: Compute Candlesticks using given dataset (Task 1)" << std::endl;
    // 8: Draw Candlesticks Plot using given dataset
    std::cout << "8: Draw Candlesticks Plot using given dataset (Task 2)" << std::endl;
    // 9: Compute Candlesticks using another dataset
    std::cout << "9: Compute Candlesticks using another dataset (Task 3)" << std::endl;
    // 10: Draw Candlesticks Graph using another dataset
    std::cout << "10: Draw Candlesticks Graph using another dataset (Task 3)" << std::endl;

    std::cout << "============== " << std::endl;

    // Avoid printing current time here as it isn't updated. s
    // std::cout << "Current time is: " << currentTime << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketStats()
{
    for (std::string const &p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                                  p, currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
    }
    // std::cout << "OrderBook contains :  " << orders.size() << " entries" << std::endl;
    // unsigned int bids = 0;
    // unsigned int asks = 0;
    // for (OrderBookEntry& e : orders)
    // {
    //     if (e.orderType == OrderBookType::ask)
    //     {
    //         asks ++;
    //     }
    //     if (e.orderType == OrderBookType::bid)
    //     {
    //         bids ++;
    //     }
    // }
    // std::cout << "OrderBook asks:  " << asks << " bids:" << bids << std::endl;
}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else
    {
        try
        {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::ask);
            obe.username = "simuser";
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else
            {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << " MerkelMain::enterAsk Bad input " << std::endl;
        }
    }
}

void MerkelMain::enterBid()
{
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else
    {
        try
        {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid);
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else
            {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }
    }
}

void MerkelMain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}

void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    for (std::string p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry &sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            if (sale.username == "simuser")
            {
                // update the wallet
                wallet.processSale(sale);
            }
        }
    }

    currentTime = orderBook.getNextTime(currentTime);
}

/**
 * @brief Computes the Candlesticks for each product, for each timeframe.
 * This function will take a long time to run, as it has to loop through every product and timeframe. 
 * 
 * @param candlesticksPerProduct specifies the number of candlesticks to be generated per Product/ProductType. 
 * If the value is set to 2, two candlesticks will be created for each distinct Product/ProductType. 
 * This parameter controls the granularity of the candlestick generation, influencing the number of 
 * candlesticks associated with each unique Product/ProductType.
 */
void MerkelMain::computeCandlesticks(int candlesticksPerProduct)
{
    // Set default value for candlesticksPerProduct if it is less than 1.
    if(candlesticksPerProduct < 1) candlesticksPerProduct = orderBook.totalTimestampCount; 

    // As this function takes a long time to run, we will add a progress meter to the console. 
    // This will be done by counting the total number of times the loop will run, and then
    // printing a message every 5% of the way through the loop.
    // As this depends heavily on std::cout remaining open and unused by other functions,
    // Do not run any std::cout within the loops of this function.
    int totalLoopCount = orderBook.getKnownProducts().size() * candlesticksPerProduct;

    // Count the number of products that has completed the loop.
    int productLoop = 0;

    // Count the number of timestamps that has completed the loop.
    int timestampLoop = 0;
    // Start the progress meter.
    printProgress(0, totalLoopCount, true);

    // Debug: Only compile for 1 singular product to reduce load time.
    std::string const &p = orderBook.getKnownProducts()[0];

    // Create 2 Candlesticks per product, per timeframe.
    // for (std::string const &p : orderBook.getKnownProducts())
    {
        currentTime = orderBook.getEarliestTime();

        double previousAskPrice;
        double previousBidPrice;

        // For progress checking
        timestampLoop = 0;

        while (true)
        {
            // Create a Candlestick for asks.
            std::vector<OrderBookEntry> askOrder = orderBook.getOrders(OrderBookType::ask, p, currentTime);
            Candlestick c1(askOrder);

            // Set the open price of the Candlestick to the previous ask price.
            // If there is no previous ask price, set the open price to the close price.
            c1.setOpenPrice(previousAskPrice == 0.0 ? c1.close : previousAskPrice);
            previousAskPrice = c1.close;
            // c1.printInformation();

            // Add the Candlestick to the vector of Candlesticks.
            candlesticks.push_back(c1);

            // Create a Candlestick for bids.
            std::vector<OrderBookEntry> bidOrder = orderBook.getOrders(OrderBookType::bid, p, currentTime);
            Candlestick c2(bidOrder);

            // Set the open price of the Candlestick to the previous bid price.
            // If there is no previous bid price, set the open price to the close price.
            c2.setOpenPrice(previousBidPrice == 0.0 ? c2.close : previousBidPrice);
            previousBidPrice = c2.close;
            // c2.printInformation();

            // Add the Candlestick to the vector of Candlesticks.
            candlesticks.push_back(c2);

            // Update the timestampLoop counter.
            timestampLoop++;

            // Print the progress meter every 5% of the way through the loop.
            printProgress(productLoop * candlesticksPerProduct + timestampLoop, totalLoopCount, false);

            // Update currentTime to the next timeframe.
            currentTime = orderBook.getNextTime(currentTime);

            // If currentTime resets to earliestTime, break out of the loop.
            if (currentTime == orderBook.getEarliestTime()) 
                break;
        }

        // Update the productLoop counter.
        productLoop++;
    }

    // Print the progress meter one last time, at 100%. 
    printProgress(1, 1, false);

    // Close the std::cout
    std::cout << std::endl;
}

/**
 * @brief Prints a progress meter to the console.
 * 
 * @param progress The current progress.
 * @param total The total number of times the loop will run.
 * @param firstIteration Whether this is the first iteration of the loop.
 */
void MerkelMain::printProgress(int progress, int total, bool firstIteration) {
    if(total == 0) return; // Prevent division by zero.

    // If this is the first iteration, print a message.
    if (firstIteration) {
        std::cout << "Progress: 0% [------------------]";
    }

    // Calculate the percentage of the progress.
    double percentage = (progress * 100) / total;

    // Move the cursor to the beginning of the line.
    std::cout << "\r";

    // Print the start of the Progress message
    std::cout << "Progress: " << percentage << "% [";

    // Print the progress bar.
    for (int i = 0; i < 20; i++) {
        if (i < (percentage / 5)) {
            std::cout << "#";
        }
        else {
            std::cout << "-";
        }
    }

    // Print the end of the Progress message
    std::cout << "]";
}

int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-6" << std::endl;
    std::getline(std::cin, line);
    try
    {
        userOption = std::stoi(line);
    }
    catch (const std::exception &e)
    {
        //
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::processUserOption(int userOption)
{
    if (userOption == 0) // bad input
    {
        std::cout << "Invalid choice. Choose 1-6" << std::endl;
    }
    if (userOption == 1)
    {
        printHelp();
    }
    if (userOption == 2)
    {
        printMarketStats();
    }
    if (userOption == 3)
    {
        enterAsk();
    }
    if (userOption == 4)
    {
        enterBid();
    }
    if (userOption == 5)
    {
        printWallet();
    }
    if (userOption == 6)
    {
        gotoNextTimeframe();
    }
    if (userOption == 7)
    {
        computeCandlesticks(0);
    }
}
