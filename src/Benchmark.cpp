#include "Benchmark.h"
#include <chrono>
#include <iostream>
#include <iomanip>

// Time Insertion Function: measures time to insert all records into RB Tree
double Benchmark::timeInsertion(RBTree &tree, const std::vector<Record> &records)
{
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto &record : records)
    {
        tree.insert(record);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

// Time Search function: measures time for a single lookup in the RB Tree
double Benchmark::timeSearch(const RBTree &tree, int date)
{
    auto start = std::chrono::high_resolution_clock::now();

    tree.search(date);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

// Time Range Query: measures time for a range query
double Benchmark::timeRangeQuery(const RBTree &tree, int startDate, int endDate)
{
    auto start = std::chrono::high_resolution_clock::now();

    tree.rangeQuery(startDate, endDate);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

// Run Report: full benchmark report for the specified ticker
void Benchmark::runReport(const std::string &ticker, const std::vector<Record> &records)
{
    std::cout << "==============================" << std::endl;
    std::cout << " Benchmark Report: " << ticker << std::endl;
    std::cout << " Records: " << records.size() << std::endl;
    std::cout << "==============================" << std::endl;

    // Insertion
    RBTree tree;
    double insertTime = timeInsertion(tree, records);
    std::cout << std::fixed << std::setprecision(4);
    std::cout << " Insertion Time:   " << insertTime << " ms" << std::endl;

    // Search (first, middle, last) date
    if (!records.empty())
    {
        int firstDate = records.front().date;
        int midDate = records[records.size() / 2].date;
        int lastDate = records.back().date;

        double searchFirst = timeSearch(tree, firstDate);
        double searchMid = timeSearch(tree, midDate);
        double searchLast = timeSearch(tree, lastDate);

        std::cout << " Search (first):   " << searchFirst << " ms" << std::endl;
        std::cout << " Search (middle):  " << searchMid << " ms" << std::endl;
        std::cout << " Search (last):    " << searchLast << " ms" << std::endl;
    }

    // Range queries (different windows)
    if (records.size() >= 2)
    {
        int startDate = records.front().date;
        int endDate = records.back().date;
        int quarterDate = records[records.size() / 4].date;
        int halfDate = records[records.size() / 2].date;

        double rangeSmall = timeRangeQuery(tree, startDate, quarterDate);
        double rangeMedium = timeRangeQuery(tree, startDate, halfDate);
        double rangeLarge = timeRangeQuery(tree, startDate, endDate);

        std::cout << " Range (25%):      " << rangeSmall << " ms" << std::endl;
        std::cout << " Range (50%):      " << rangeMedium << " ms" << std::endl;
        std::cout << " Range (100%):     " << rangeLarge << " ms" << std::endl;
    }

    // Stats function for full range
    std::vector<Record> allResults = tree.rangeQuery(records.front().date, records.back().date);
    std::cout << " Moving Average:   " << Stats::movingAverage(allResults) << std::endl;
    std::cout << " Volatility:       " << Stats::volatility(allResults) << std::endl;
    std::cout << " Price Change:     $" << Stats::priceChange(allResults) << std::endl;
    std::cout << " Percent Change:   " << Stats::percentChange(allResults) << "%" << std::endl;
    std::cout << "==============================" << std::endl;
}