#ifndef CSVLOADER_H
#define CSVLOADER_H

#include "RBTree.h"
#include <string>
#include <map>
#include <vector>


class CSVLoader {
public:
    // Loads CSV and returns a <map> of ticker using an RBTree
    static std::map<std::string, RBTree> loadCSV(const std::string& filepath);

private:
    // Removes dashes in dates
    static int parseDate(const std::string& dateStr);

    // Splits entries using the comma as a reference
    static std::vector<std::string> splitLine(const std::string& line, char delimiter);


};

#endif