#include "CSVLoader.h"
#include "Stats.h"
#include "Benchmark.h"
#include <iostream>
#include <string>

int main() {
    std::string filepath = "../data/world_stock_prices.csv";

    // Loads CSV into map of RBTrees
    std::cout << "Loading CSV..." << std::endl;
    std::map<std::string, RBTree> trees = CSVLoader::loadCSV(filepath);
    std::cout << "Loaded " << trees.size() << " tickers." << std::endl;

    // Menu Loop 
    std::string input;
    while (true) {
        std::cout << "\n===== Red-Black Tree Stock Tool =====" << std::endl;
        std::cout << "1. Query a ticker" << std::endl;
        std::cout << "2. Run benchmark on a ticker" << std::endl;
        std::cout << "3. List all tickers" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Choice: ";
        std::getline(std::cin, input);

        if (input == "1") {
            // 1. Query a ticker
            std::string ticker;
            std::cout << "Enter a ticker (ex: AAPL): ";
            std::getline(std::cin, ticker);

            if (trees.find(ticker) == trees.end()) {
                std::cout << "Ticker not found." << std::endl;
                continue;
            }
            std::string startStr, endStr;
            std::cout << "Enter start date (YYYYMMDD): ";
            std::getline(std::cin, startStr);
            std::cout << "Enter end date   (YYYYMMDD): ";
            std::getline(std::cin, endStr);

            int startDate = std::stoi(startStr);
            int endDate = std::stoi(endStr);

            std::vector<Record> results = trees[ticker].rangeQuery(startDate, endDate);

            std::cout << "\nResults: " << results.size() << " records found." << std::endl;
            std::cout << " Moving Average:  " << Stats::movingAverage(results) << std::endl;
            std::cout << " Volatility:      " << Stats::volatility(results) << std::endl;
            std::cout << " Price Change:    $" << Stats::priceChange(results) << std::endl;
            std::cout << " Percent Change:  " << Stats::percentChange(results) << "%" << std::endl;
        }

        else if (input == "2") {
            // 2. Benchmark for Ticker
            std::string ticker;
            std::cout << "Enter a ticker (ex: AAPL): ";
            std::getline(std::cin, ticker);

            if (trees.find(ticker) == trees.end()) {
                std::cout << "Ticker not found." << std::endl;
                continue;
            }

            // Get all records from the tree to feed into benchmark
            RBTree& tree = trees[ticker];
            int firstDate = 19000101;
            int lastDate = 20270101;
            std::vector<Record> allRecords = tree.rangeQuery(firstDate, lastDate);

            Benchmark::runReport(ticker, allRecords);
        }

        else if (input == "3") {
            // 3. List All Tickers
            std::cout << "\nLoaded tickers:" << std::endl;
            int count = 0;
            for (const auto& pair : trees) {
                std::cout << " " << pair.first;
                count++;
                if (count % 10 == 0) {
                    std::cout << std::endl;
                }
            }
            std::cout << std::endl;
        }

        else if (input == "4") {
            std::cout << "Exiting." << std::endl;
            break;
        }
        else {
            std::cout << "Invalid choice." << std::endl;
        }
    }
    return 0;
}
