#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "RBTree.h"
#include "Stats.h"
#include <string>
#include <vector>

class Benchmark {
public:
    // Times how long it takes to insert all records into RB tree
    static double timeInsertion(RBTree& tree, const std::vector<Record>& records);

    // Times a singular search by date
    static double timeSearch(const RBTree& tree, int date);

    // Times a range query b/t 2 dates 
    static double timeRangeQuery(const RBTree& tree, int startDate, int endDate);

    // Runs a benchmark report for a singular tickers RB tree
    static void runReport(const std::string& ticker, const std::vector<Record>& records);
};

#endif