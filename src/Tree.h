#ifndef TREE_H
#define TREE_H

#include "Record.h"
#include <string>
#include <vector>

class Tree
{
public:
    virtual ~Tree() = default;

    virtual void insert(const Record &record) = 0;
    virtual std::vector<Record> rangeQuery(int startDate, int endDate) const = 0;

    void load(const std::string &filepath, const std::string &tickerFilter);
};

#endif