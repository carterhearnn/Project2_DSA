#ifndef RBTREE_H
#define RBTREE_H

#include "RBNode.h"
#include "Tree.h"
#include <vector>

class RBTree : public Tree
{
private:
    RBNode *root;
    int nodeCount;

    // Rotation Helper Functions
    void rotateLeft(RBNode *node);
    void rotateRight(RBNode *node);

    // Fixing issues after insertion
    void insertFixup(RBNode *node);

    // Helper for range query
    void rangeQueryHelper(RBNode *node, int startDate, int endDate, std::vector<Record> &results) const;

    // cleanup
    void destroyTree(RBNode *node);

public:
    RBTree();
    ~RBTree() override;

    // Insert & Search Functions
    void insert(const Record &record) override;
    RBNode *search(int date) const;

    // Range Query: returns records between Start/End Date
    std::vector<Record> rangeQuery(int startDate, int endDate) const override;

    // other functions
    int size() const;
    bool isEmpty() const;
};

#endif