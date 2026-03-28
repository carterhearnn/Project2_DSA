#ifndef BPTREE_H
#define BPTREE_H

#include "Record.h"
#include "Tree.h"
#include <vector>

class BPlusTree : public Tree
{
private:
    struct Node;

    Node *root;
    int order; // max children for internal nodes, max keys/records per leaf = order - 1

    int maxKeys() const;
    Node *findLeaf(int key) const;
    void clear(Node *node);
    void insertIntoLeaf(Node *leaf, const Record &record);
    std::pair<int, Node *> splitLeaf(Node *leaf);
    void insertIntoInternal(Node *internal, int key, Node *rightChild);
    std::pair<int, Node *> splitInternal(Node *internal);
    bool insertRecursive(Node *node, const Record &record, int &promotedKey, Node *&newRightChild);

public:
    explicit BPlusTree(int order = 4);
    BPlusTree(const BPlusTree &) = delete;
    BPlusTree &operator=(const BPlusTree &) = delete;
    BPlusTree(BPlusTree &&other) noexcept;
    BPlusTree &operator=(BPlusTree &&other) noexcept;
    ~BPlusTree() override;

    void insert(const Record &record) override;
    Record *search(int date);
    const Record *search(int date) const;
    bool contains(int date) const;
    std::vector<Record> rangeQuery(int startDate, int endDate) const override;
};

#endif
