#include "CSVLoader.h"
#include <sstream>
#include <fstream>
#include <iostream>

// Load CSV Functionality (1 RBTree per Ticker)

std::map<std::string, RBTree> CSVLoader::loadCSV(const std::string& filepath) {
    std::map<std::string, RBTree> trees;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        return trees;
    }

    // Skip the header row 
    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::vector<std::string> fields = splitLine(line, ',');

        // We have 8 fields: Date, Open, High, Low, Close, Volume, Brand_Name, Ticker
        if (fields.size() < 8) continue;

        // Separate the fields -> Declare values with corresponding index
        int date = parseDate(fields[0]);
        double open = std::stod(fields[1]);
        double high = std::stod(fields[2]);
        double low = std::stod(fields[3]);
        double close = std::stod(fields[4]);
        double volume = std::stod(fields[5]);
        std::string ticker = fields[7];

        // Build record -> insert into tree
        Record record(date , open, high, low, close, volume);
        trees[ticker].insert(record);

    }
    file.close();
    return trees;
}

// Remove dashes from date using parse function
int CSVLoader::parseDate(const std::string& dateStr) {
    // Pull first 10 values of string
    std::string dateOnly = dateStr.substr(0,10);

    // Remove dashes
    std::string clean = "";
    for (char c : dateOnly) {
        if (c != '-') {
            clean += c;
        }
    }

    return std::stoi(clean);
}

// Split string into tokens using the comma as the delimiter
std::vector<std::string> CSVLoader::splitLine(const std::string& line, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}