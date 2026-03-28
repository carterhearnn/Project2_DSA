#include "CSVUtils.h"
#include <sstream>

int parseDate(const std::string &dateStr)
{
    std::string dateOnly = dateStr.substr(0, 10);

    std::string clean;
    clean.reserve(dateOnly.size());
    for (char c : dateOnly)
    {
        if (c != '-')
        {
            clean += c;
        }
    }

    return std::stoi(clean);
}

std::vector<std::string> splitLine(const std::string &line, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}
