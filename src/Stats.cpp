#include "Stats.h"
#include <cmath>

// Moving Average: average of closing prices within date range
double Stats::movingAverage(const std::vector<Record>& records) {
    if (records.empty()) {
        return 0.0;
    }

    double sum = 0.0;
    for (const auto& record : records) {
        sum += record.close;
    }

    return sum / records.size();
}

// Volatility: standard deviation of returns 
double Stats::volatility(const std::vector<Record>& records) {

    // Require at least 2 records to calculate the returns
    if (records.size() < 2) {
        return 0.0;
    }

    // Calculate Returns
    std::vector<double> returns;
    for (size_t i = 1; i < records.size(); i++) {
        if (records[i - 1].close != 0.0) {
            double dailyReturn = (records[i].close - records[i - 1].close) / records[i - 1].close;
            returns.push_back(dailyReturn);
        }
    }

    // Validate there is a value within the record
    if (returns.empty()) {
        return 0.0;
    }

    // Calculate average of returns
    double sum = 0.0;
    for (double r : returns) {
        sum += r;
    }
    double mean = sum / returns.size();

    // Calculate variance -> Must square r - mean to ensure a positive value
    double variance = 0.0;
    for (double r : returns) {
        variance += ((r - mean) * (r - mean));
    }
    variance /= returns.size();

    // Standard Deviation calculation
    return std::sqrt(variance);

}

// Price Change: $ difference from first to last closing price in the date range
double Stats::priceChange(const std::vector<Record>& records) {
    if (records.size() < 2) {
        return 0.0;
    }

    return records.back().close - records.front().close;
}


// Percent Change: price change as a percentage of the original value of the stock
double Stats::percentChange(const std::vector<Record>& records) {
    if (records.size() < 2) {
        return 0.0;
    }

    double startPrice = records.front().close;
    if (startPrice == 0.0) {
        return 0.0;
    }

    return ((records.back().close - startPrice) / startPrice) * 100.0;
}