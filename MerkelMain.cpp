#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <fstream>

#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "Candlestick.h"
#include "OrderBook.h"

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
	// Start of Original Code without Assistance
	std::cout << std::endl; // Leave a line in the console.
	std::cout << "Menu:" << std::endl;
	std::cout << "1: Print help                                           		2: Print exchange stats" << std::endl;
	std::cout << "3: Make an offer                                        		4: Make a bid" << std::endl;
	std::cout << "5: Print wallet                                         		6: Continue" << std::endl;
	std::cout << "7: Compute Candlesticks using given dataset (Task 1)    		8: Draw Candlesticks Plot using given dataset (Task 2)" << std::endl;
	std::cout << "9: Compute Candlesticks using another dataset (Task 3)  		10: Draw Candlesticks Graph using another dataset (Task 3)" << std::endl;
	// End of Original Code without Assistance
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

// Start of Original Code without Assistance

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
	// Check if the user wants to compute Candlesticks.
	std::cout << "\nThis function will take a long time to run. " << std::endl;
	std::cout << "Confirm? (Y/N): ";

	// Get user input
	std::string input;
	std::getline(std::cin, input);

	// Check if the user wants to compute Candlesticks.
	if (input != "Y" && input != "y")
	{
		// Return to main menu
		return;
	}

	std::cout << "\nComputing Candlesticks..." << std::endl;

	// Set default value for candlesticksPerProduct if it is less than 1.
	if (candlesticksPerProduct < 1)
		candlesticksPerProduct = orderBook.totalTimestampCount;

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
	// std::string const &p = orderBook.getKnownProducts()[0];

	// Create 2 Candlesticks per product, per timeframe.
	for (std::string const &p : orderBook.getKnownProducts())
	{
		// currentTime = orderBook.getEarliestTime();
		// Create a temporary vector of Candlesticks.
		std::vector<Candlestick> tempCandlestick;

		double previousAskPrice = 0.0;
		double previousBidPrice = 0.0;

		// For progress checking
		timestampLoop = 0;

		// Set previous timeframe index to 0
		int previousTimeframeIndex = 0;

		// while (true)
		for (int i = 1; i <= candlesticksPerProduct; i++)
		{
			// Get all timeframes for current iteration
			// Get end index
			double end = (i * orderBook.allTimestamps.size()) / candlesticksPerProduct;
			std::set<std::string> timeframes(std::next(orderBook.allTimestamps.begin(), previousTimeframeIndex), std::next(orderBook.allTimestamps.begin(), static_cast<std::size_t>(end)));

			// Create vectors to store all askOrders and bidOrders
			std::vector<OrderBookEntry> askOrder;
			std::vector<OrderBookEntry> bidOrder;

			// Get all askOrders and bidOrders
			for (std::string const &t : timeframes)
			{
				// Get all askOrders and bidOrders for current timeframe
				std::vector<OrderBookEntry> tempAskOrder = orderBook.getOrders(OrderBookType::ask, p, t);
				std::vector<OrderBookEntry> tempBidOrder = orderBook.getOrders(OrderBookType::bid, p, t);

				// Add all askOrders and bidOrders to the vector of askOrders and bidOrders
				if (!tempAskOrder.empty())
					askOrder.insert(askOrder.end(), tempAskOrder.begin(), tempAskOrder.end());

				if (!tempBidOrder.empty())
					bidOrder.insert(bidOrder.end(), tempBidOrder.begin(), tempBidOrder.end());
			}

			// Set previousTimeframeIndex to the current timeframe index
			previousTimeframeIndex = static_cast<std::size_t>(end);

			// Create a Candlestick for asks.
			// std::vector<OrderBookEntry> askOrder = orderBook.getOrders(OrderBookType::ask, p, currentTime);
			Candlestick c1(askOrder);

			// Set the open price of the Candlestick to the previous ask price.
			// If there is no previous ask price, set the open price to the close price.
			c1.setOpenPrice(previousAskPrice == 0.0 ? c1.close : previousAskPrice);
			previousAskPrice = c1.close;
			// c1.printInformation();

			// Add the Candlestick to the vector of Candlesticks.
			tempCandlestick.push_back(c1);

			// Create a Candlestick for bids.
			// std::vector<OrderBookEntry> bidOrder = orderBook.getOrders(OrderBookType::bid, p, currentTime);
			Candlestick c2(bidOrder);

			// Set the open price of the Candlestick to the previous bid price.
			// If there is no previous bid price, set the open price to the close price.
			c2.setOpenPrice(previousBidPrice == 0.0 ? c2.close : previousBidPrice);
			previousBidPrice = c2.close;
			// c2.printInformation();

			// Add the Candlestick to the vector of Candlesticks.
			tempCandlestick.push_back(c2);

			// Update the timestampLoop counter.
			timestampLoop++;

			// Print the progress meter every 5% of the way through the loop.
			printProgress(productLoop * candlesticksPerProduct + timestampLoop, totalLoopCount, false);

			// // Update currentTime to the next timeframe.
			// currentTime = orderBook.getNextTime(currentTime);

			// // If currentTime resets to earliestTime, break out of the loop.
			// if (currentTime == orderBook.getEarliestTime())
			//     break;
		}

		// Add the vector of Candlesticks to the vector of vectors of Candlesticks.
		candlesticks.push_back(tempCandlestick);

		// Update the productLoop counter.
		productLoop++;
	}

	// Print the progress meter one last time, at 100%.
	printProgress(1, 1, false);

	// Close the std::cout
	std::cout << std::endl;
}

void MerkelMain::computeCustomCandlesticks()
{
	// Verify candlestickBook is not empty
	if (candlestickBook.allTimestamps.empty())
	{
		std::cout << "No Candlesticks to display!" << std::endl;
		return;
	}

	// Check if there's a backup
	if (backup.size() == 0)
	{
		// Backup the candlesticks
		backup = candlestickBook.candlesticks;
	}
	else
	{
		// Restore the backup first
		candlestickBook.candlesticks = backup;
	}

	// Ask the user for the number of Candlesticks to be generated
	std::cout << "\nEnter the number of Candlesticks to be generated: ";

	std::cout << candlestickBook.candlesticks.size() << std::endl;

	// Get user input
	std::string input;
	std::getline(std::cin, input);
	int candlesticksPerProduct;
	try
	{
		candlesticksPerProduct = std::stoi(input);
	}
	catch (const std::exception &e)
	{
		std::cout << "Invalid input!" << std::endl;
		return;
	}

	// Adjust the CandlestickBook to contain the desired number of Candlesticks
	candlestickBook.ReduceCandlesticks(candlesticksPerProduct);

	for (Candlestick &cs : candlestickBook.candlesticks)
	{
		cs.printInformation();
	}
}

/**
 * @brief Prints a progress meter to the console.
 *
 * @param progress The current progress.
 * @param total The total number of times the loop will run.
 * @param firstIteration Whether this is the first iteration of the loop.
 */
void MerkelMain::printProgress(int progress, int total, bool firstIteration)
{
	if (total == 0)
		return; // Prevent division by zero.

	// Set default number of progress bars to 30.
	double progressBarCount = 30;
	double progressAmount = 100 / progressBarCount;

	// If this is the first iteration, open the std::cout.
	if (firstIteration)
	{
		std::cout << "";
	}

	// Calculate the percentage of the progress.
	double percentage = (progress * 100) / total;

	// Move the cursor to the beginning of the line.
	std::cout << "\r";

	// Print the start of the Progress message
	std::cout << ((percentage == 100) ? "Completed: " : "Progress: ") << ceil(percentage) << "% [";

	// Print the progress bar.
	for (int i = 0; i < progressBarCount; i++)
	{
		if (i < (percentage / progressAmount))
		{
			std::cout << "#";
		}
		else
		{
			std::cout << "-";
		}
	}

	// Print the end of the Progress message
	std::cout << "]";
}

/**
 * @brief Asks the user on which product to display Candlesticks.
 *
 */
void MerkelMain::requestCandlesticks()
{
	// Check if there are any Candlesticks to display.
	if (candlesticks.empty())
	{
		std::cout << "\nNo Candlesticks to display!" << std::endl;

		// Check if the user wants to compute Candlesticks.
		std::cout << "Compute Candlesticks first? (Y/N): ";

		// Get user input
		std::string input;
		std::getline(std::cin, input);

		// Check if the user wants to compute Candlesticks.
		if (input == "Y" || input == "y")
		{
			// Compute Candlesticks
			computeCandlesticks(5);
		}
		else
		{
			// Return to main menu
			return;
		}
	}

	// Ask the user for which product to display Candlesticks.
	std::cout << "\nEnter the product to display Candlesticks: " << std::endl;

	// Print all available products
	int index = 1; // Index of the product
	// for (std::string const &p : orderBook.getKnownProducts())
	// {
	// 	std::cout << index << ": " << p << std::endl;
	// 	index++;
	// }

	for (std::vector<Candlestick> const &c : candlesticks)
	{
		std::cout << index << ": " << c[0].product << std::endl;
		index++;
	}

	// Get user input
	std::string input;
	std::getline(std::cin, input);
	try
	{
		index = std::stoi(input);
	}
	catch (const std::exception &e)
	{
		std::cout << "Invalid input!" << std::endl;
		return;
	}

	// Check if the index is valid
	if (index < 1 || index > candlesticks.size())
	{
		std::cout << "Invalid input!" << std::endl;
		return;
	}

	std::cout << "Displaying Candlesticks for " << candlesticks[index - 1][0].product << std::endl;

	// Draw the Candlesticks
	drawCandlesticks(candlesticks[index - 1]);
}

/**
 * @brief Prepares the Candlesticks for custom product to be drawn.
 *
 */
void MerkelMain::requestCustomCandlesticks()
{
	// Check if there are any Candlesticks to display.
	if (candlestickBook.candlesticks.size() <= 0)
	{
		std::cout << "\nNo Candlesticks to display!" << std::endl;

		// Check if the user wants to compute Candlesticks.
		std::cout << "Compute Candlesticks first? (Y/N): ";

		// Get user input
		std::string input;
		std::getline(std::cin, input);

		// Check if the user wants to compute Candlesticks.
		if (input == "Y" || input == "y")
		{
			// Compute Candlesticks
			computeCustomCandlesticks();
		}
		else
		{
			// Return to main menu
			return;
		}
	}
	// Draw the Candlesticks
	drawCandlesticks(candlestickBook.candlesticks);
}

/**
 * @brief Draws a text-based graph of the Candlesticks.
 *
 * @param candlesticks vector of Candlesticks to be drawn.
 */
void MerkelMain::drawCandlesticks(std::vector<Candlestick> candlesticks)
{
	// Check if there are any Candlesticks to display.
	if (candlesticks.empty())
	{
		std::cout << "\nNo Candlesticks received!" << std::endl;

		// As this error is code related, we will not ask the user to compute Candlesticks.
		// Return to main menu
		return;
	}

	if (candlesticks.size() >= 15)
	{
		std::cout << "Too many candlesticks to display!" << std::endl;
		return;
	}

	// Create a string of Vectors to store the output. Each Vector represents a line in the output.
	std::vector<std::string> display;

	// Create a reusable temp string to store temporary values.
	std::string temp;

	// Declare the settings of the graph space
	// paddingX is the number of spaces between the Candlesticks and the left and right edge.
	int paddingX = 3;

	// paddingY is the number of spaces between the Candlesticks and the top and bottom edge.
	int paddingY = 1;

	// leftColumnWidth is the width of the left column, which displays the price of the Candlesticks.
	int leftColumnWidth = 9;

	// yPlotCount is the number of plotted price values on the left
	int yPlotCount = 8;

	// yPlotGap is the interval between each plotted price value on the left
	int yPlotGap = 4;

	// Declare the settings of the plot itself
	// gapWidth is the size of the gap between each Candlestick.
	int gapWidth = 7;

	// candlestickWidth is the width of each Candlestick. This must be an even number.
	int candlestickWidth = 7;
	// Check if candlestickWidth is an even number, and if not, make it an even number.
	if (candlestickWidth % 2 == 0)
		candlestickWidth++;

	// Calculate the details of the plot.
	// Calculate the width of the plot, without including the left column.
	int plotWidth = candlesticks.size() * candlestickWidth + (candlesticks.size() - 1) * gapWidth + paddingX * 2;

	// Get the highest and lowest price of the Candlesticks.
	double highestPrice = 0.0;
	double lowestPrice = candlesticks[0].low;

	for (Candlestick const &c : candlesticks)
	{
		if (c.high > highestPrice)
			highestPrice = c.high;
		if (c.low < lowestPrice)
			lowestPrice = c.low;
	}

	// Calculate the range of the Candlesticks.
	double range = highestPrice - lowestPrice;

	// Calculate the price interval between each plotted price value on the left.
	double yPlotInterval = range / (yPlotCount - 1);

	// Reserve the size of the display array to help with memory allocation.
	display.reserve(3 + (yPlotCount - 1) * yPlotGap + paddingY * 2);

	// Draw the left column into the display array
	// Draw the top edge of the graph
	display.push_back(candlesticks[0].product);

	// Draw the top padding of the graph
	for (int i = 0; i < paddingY; i++)
	{
		temp = "";
		for (int j = 0; j < leftColumnWidth; j++)
			temp += " ";

		// Draw the Y Axis
		temp += "|";

		// Add to display
		display.push_back(temp);
	}

	// Store the index of the top of the graph
	int topIndex = display.size();

	// Draw the Y Axis
	for (int i = 0; i < yPlotCount; i++)
	{
		// Clear temp
		temp = "";

		double value = highestPrice - i * yPlotInterval;

		// Format the plotted price, and limit the decimal point to 2
		std::string temp = std::to_string(value);
		size_t decimalPos = temp.find(".");
		if (decimalPos != std::string::npos && temp.length() > decimalPos + 2)
		{
			temp.erase(decimalPos + 3);
		}

		// Add spaces to the end of the value to make it the same length as the leftColumnWidth
		for (int i = 0; i < leftColumnWidth - temp.length();)
			temp += " ";

		// Draw the Y Axis
		temp += "|";

		for (int j = 0; j < paddingX; j++)
			temp += " ";

		// Add to display
		display.push_back(temp);

		// Draw the Y axis for the gaps between the plotted price values
		for (int j = 0; j < yPlotGap; j++)
		{
			// Clear temp
			temp = "";

			// Add spaces to the end of the value to make it the same length as the leftColumnWidth
			for (int i = 0; i < leftColumnWidth; i++)
				temp += " ";

			// Draw the Y Axis
			temp += "|";

			for (int k = 0; k < paddingX; k++)
				temp += " ";

			// Add to display
			display.push_back(temp);
		}
	}

	// Store the index of the bottom of the graph
	int bottomIndex = display.size() - 1;

	// Draw the bottom padding of the graph
	for (int i = 0; i < paddingY; i++)
	{
		temp = "";
		for (int j = 0; j < leftColumnWidth; j++)
			temp += " ";

		// Draw the Y Axis
		temp += "|";

		// Add to display
		display.push_back(temp);
	}

	// Draw the bottom edge of the graph
	// Get the last line of the display array and add the bottom edge of the graph to it.
	temp = display.back();
	for (int i = 0; i < plotWidth; i++)
		temp += "_";

	// Save changes to display
	display.back() = temp;

	// Draw the X axis
	temp = "";
	temp = "DATE";
	for (int i = 0; i < leftColumnWidth - temp.length();)
	{
		temp += " ";
	}
	temp += "|";

	for (int i = 0; i < paddingX; i++)
		temp += " ";

	// Add to display
	display.push_back(temp);

	// Save the index of Date to be used later
	int dateIndex = display.size() - 1;

	// Draw 2nd part of X axis
	temp = "TIME";
	for (int i = 0; i < leftColumnWidth - temp.length();)
		temp += " ";
	temp += "|";

	for (int i = 0; i < paddingX; i++)
		temp += " ";

	// Add to display
	display.push_back(temp);

	// Draw 3rd part of X axis
	temp = "TYPE";
	for (int i = 0; i < leftColumnWidth - temp.length();)
		temp += " ";
	temp += "|";

	for (int i = 0; i < paddingX; i++)
		temp += " ";

	// Add to display
	display.push_back(temp);

	// Get the y interval between each line
	double yInterval = range / ((yPlotCount - 1) * (yPlotGap + 1) + 1);

	// Draw the Candlesticks
	for (Candlestick &c : candlesticks)
	{
		// c.printInformation();

		for (int i = topIndex; i <= bottomIndex; i++)
		{
			// Clear temp
			temp = "";

			double price = highestPrice - (i - topIndex) * yInterval;
			// Check if the price is within which range of the candlestick.
			if (price - yInterval / 2 <= c.high && price + yInterval / 2 >= c.low)
			{
				// Get the higher/lower price of open/close price
				double higherPrice = c.open > c.close ? c.open : c.close;
				double lowerPrice = c.open < c.close ? c.open : c.close;

				// Check if the price is within range of open/close price
				if (price - yInterval / 2 <= c.open && price + yInterval / 2 >= c.open)
				{
					// Check if the price is within range of close price as well
					if (price - yInterval / 2 <= c.close && price + yInterval / 2 >= c.close && c.open < c.close)
					{
						// Draw the open price
						temp += "|";
						for (int j = 2; j < candlestickWidth; j++)
							temp += "+";
						temp += "|";

						// Draw the gap between candlesticks
						for (int j = 0; j < gapWidth; j++)
							temp += " ";
					}
					else
					{
						// Draw the open price
						temp += "|";
						for (int j = 2; j < candlestickWidth; j++)
							temp += "+";
						temp += "|";

						// Draw the gap between candlesticks
						for (int j = 0; j < gapWidth; j++)
							temp += " ";
					}
					// // Draw the open price
					// temp += "|";
					// for (int j = 2; j < candlestickWidth; j++)
					// 	temp += "+";
					// temp += "|";

					// // Draw the gap between candlesticks
					// for (int j = 0; j < gapWidth; j++)
					// 	temp += " ";
				}
				else if (price - yInterval / 2 <= c.close && price + yInterval / 2 >= c.close)
				{
					// Draw the close price
					temp += "|";
					for (int j = 2; j < candlestickWidth; j++)
						temp += "-";
					temp += "|";

					// Draw the gap between candlesticks
					for (int j = 0; j < gapWidth; j++)
						temp += " ";
				}
				// Check if the price is between open/close price
				else if (price - yInterval / 2 <= higherPrice && price + yInterval / 2 >= lowerPrice)
				{
					// Draw the body of the candlestick
					temp += "|";
					for (int j = 2; j < candlestickWidth; j++)
						temp += " ";
					temp += "|";

					// Draw the gap between candlesticks
					for (int j = 0; j < gapWidth; j++)
						temp += " ";
				}
				else
				{
					// Draw the body of the candlestick
					for (int j = 0; j < floor(candlestickWidth / 2); j++)
						temp += " ";

					temp += "|";

					for (int j = 0; j < floor(candlestickWidth / 2) + gapWidth; j++)
						temp += " ";
				}
			}
			// If not in range, draw spaces
			else
			{
				for (int j = 0; j < candlestickWidth + gapWidth; j++)
					temp += " ";
			}

			// Save changes to display
			display[i] += temp;
		}

		// Add the Date, Time and Type
		// Add the Date
		// Check if the timestamp is long enough to contain the date
		if (c.timestamp.length() < 10)
			// If it's too short, use the entire timestamp instead.
			temp = c.timestamp;
		else
			temp = c.timestamp.substr(0, 9);

		for (int i = 0; i < (ceil(candlestickWidth) + gapWidth) - temp.length();)
			temp += " ";

		// Add to display
		display[dateIndex] += temp;

		// Add the Time
		// Check if the timestamp is long enough to contain the time
		if (c.timestamp.length() < 11)
			temp = "";
		else
			temp = c.timestamp.substr(11, 8);

		for (int i = 0; i < (ceil(candlestickWidth) + gapWidth) - temp.length();)
			temp += " ";

		// Add to display
		display[dateIndex + 1] += temp;

		// Add the Type
		temp = orderBookTypeToString(c.orderType);
		for (int i = 0; i < (ceil(candlestickWidth) + gapWidth) - temp.length();)
			temp += " ";

		// Add to display
		display[dateIndex + 2] += temp;
	}

	// Leave a line before drawing the Candlesticks
	std::cout << std::endl;

	// Print the display array into the console
	for (std::string const &s : display)
		std::cout << s << std::endl;

	// Save the display array into a file
	writeToFile(display, "Candlesticks.txt");
}

/**
 * @brief Write a vector of strings into an output file.
 *
 * @param data std::vector<std::string> to be written into the file.
 * @param filename Name of the file to be written into.
 */
void MerkelMain::writeToFile(const std::vector<std::string> &data, const std::string &filename)
{
	// Open the file
	std::ofstream outputFile(filename, std::ios::out | std::ios::trunc);

	if (outputFile.is_open())
	{
		for (const std::string &line : data)
		{
			outputFile << line << '\n';
		}

		outputFile.close();
		std::cout << "\nData successfully written to " << filename << std::endl;
	}
	else
	{
		std::cerr << "Unable to open file: " << filename << std::endl;
	}
}

// End of Original Code without Assistance

int MerkelMain::getUserOption()
{
	int userOption = 0;
	std::string line;
	std::cout << "Choose an option: ";
	std::getline(std::cin, line);
	try
	{
		userOption = std::stoi(line);
	}
	catch (const std::exception &e)
	{
		//
	}
	// std::cout << "You chose: " << userOption << std::endl;
	return userOption;
}

void MerkelMain::processUserOption(int userOption)
{
	if (userOption == 0) // bad input
	{
		std::cout << "Invalid choice. Choose again." << std::endl;
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
		computeCandlesticks(5);
	}
	if (userOption == 8)
	{
		requestCandlesticks();
	}
	if (userOption == 9)
	{
		computeCustomCandlesticks();
	}
	if (userOption == 10)
	{
		requestCustomCandlesticks();
	}
}
