#include "Tree.h"
#include "CSVUtils.h"
#include <fstream>
#include <iostream>

void Tree::load(const std::string &filepath, const std::string &tickerFilter)
{
    std::ifstream file(filepath);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        return;
    }

    // Skip the header row
    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::vector<std::string> fields = splitLine(line, ',');

        // We have 8 fields: Date, Open, High, Low, Close, Volume, Brand_Name, Ticker
        if (fields.size() < 8)
            continue;

        // Separate the fields -> Declare values with corresponding index
        int date = parseDate(fields[0]);
        double open = std::stod(fields[1]);
        double high = std::stod(fields[2]);
        double low = std::stod(fields[3]);
        double close = std::stod(fields[4]);
        double volume = std::stod(fields[5]);
        std::string ticker = fields[7];

        if (ticker != tickerFilter)
            continue;

        // Build record -> insert into tree
        Record record(date, open, high, low, close, volume);

        insert(record);
    }
}
