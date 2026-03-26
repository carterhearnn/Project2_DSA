#ifndef STATS_H
#define STATS_H

#include "Record.h"
#include <vector>

class Stats {
public:
    // Average closing price over a range
    static double movingAverage(const std::vector<Record>& records);

    // Standard deviation of daily returns (aka volatility)
    static double volatility(const std::vector<Record>& records);

    // Price change from first to last record in range
    static double priceChange(const std::vector<Record>& records);

    // Percentage change from first to last record in range
    static double percentChange(const std::vector<Record>& records);
    
};

#endif 