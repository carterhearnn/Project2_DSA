#ifndef CSVUTILS_H
#define CSVUTILS_H

#include <string>
#include <vector>

int parseDate(const std::string &dateStr);
std::vector<std::string> splitLine(const std::string &line, char delimiter);

#endif
