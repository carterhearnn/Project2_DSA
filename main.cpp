#include "src/BPTree.h"
#include "src/CSVUtils.h"
#include "src/RBTree.h"
#include "src/Record.h"

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::cerr << "Usage: " << argv[0] << " <BP|RB> <stock_ticker> <start_date_utc> <end_date_utc>" << std::endl;
        std::cerr << "Example: " << argv[0] << " RB AAPL 2024-01-01 2024-01-31" << std::endl;
        return 1;
    }

    const std::string dsType = argv[1];
    const std::string ticker = argv[2];

    int startDate = 0;
    int endDate = 0;
    try
    {
        startDate = parseDate(argv[3]);
        endDate = parseDate(argv[4]);
    }
    catch (...)
    {
        std::cerr << "Invalid date format. Use YYYY-MM-DD or YYYYMMDD." << std::endl;
        return 1;
    }

    if (startDate > endDate)
    {
        std::cerr << "Start date must be less than or equal to end date." << std::endl;
        return 1;
    }

    const std::string filepath = "data/world_stock_prices.csv";

    auto printResults = [](const std::vector<Record> &results)
    {
        std::cout << "date,open,high,low,close,volume" << std::endl;
        for (const Record &record : results)
        {
            std::cout
                << record.date << ","
                << record.open << ","
                << record.high << ","
                << record.low << ","
                << record.close << ","
                << record.volume << std::endl;
        }
    };

    if (dsType == "RB")
    {
        RBTree tree;
        tree.load(filepath, ticker);

        auto queryStart = std::chrono::high_resolution_clock::now();
        const std::vector<Record> results = tree.rangeQuery(startDate, endDate);
        auto queryEnd = std::chrono::high_resolution_clock::now();
        auto queryNs = std::chrono::duration_cast<std::chrono::nanoseconds>(queryEnd - queryStart).count();

        std::cout << queryNs << std::endl;
        printResults(results);
    }
    else if (dsType == "BP")
    {
        BPlusTree tree;
        tree.load(filepath, ticker);

        auto queryStart = std::chrono::high_resolution_clock::now();
        const std::vector<Record> results = tree.rangeQuery(startDate, endDate);
        auto queryEnd = std::chrono::high_resolution_clock::now();
        auto queryNs = std::chrono::duration_cast<std::chrono::nanoseconds>(queryEnd - queryStart).count();

        std::cout << queryNs << std::endl;
        printResults(results);
    }
    else
    {
        std::cerr << "Datastructure must be one of BP or RB" << std::endl;
        return 1;
    }

    return 0;
}