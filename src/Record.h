#ifndef RECORD_H
#define RECORD_H

#include <string>

struct Record {
    int date;
    double open;
    double high;
    double low;
    double close;
    double volume;

    // Default Constructor
    Record() : date(0), open(0), high(0), low(0), close(0), volume(0) {}

    // Parameterized Constructor
    Record (int date, double open, double high, double low, double close, double volume) 
            : date(date), open(open), high(high), low(low), close(close), volume(volume) {}

    // Compare using the data for insertion/search
    bool operator < (const Record& other) const { return date < other.date; }
    bool operator > (const Record& other) const { return date > other.date; }
    bool operator == (const Record& other) const { return date == other.date; }

};

#endif