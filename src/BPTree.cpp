#include "BPTree.h"
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <queue>
#include <stdexcept>

struct BPlusTree::Node
{
    bool isLeaf;
    std::vector<int> keys;
    std::vector<Node *> children;
    std::vector<Record> records;
    Node *next;
    explicit Node(bool leaf = false) : isLeaf(leaf), next(nullptr) {}
};

int BPlusTree::maxKeys() const
{
    return order - 1;
}

// finds leaf that has key recursively.
BPlusTree::Node *BPlusTree::findLeaf(int key) const
{
    if (root == nullptr)
    {
        return nullptr;
    }
    Node *current = root;
    while (!current->isLeaf)
    {
        std::size_t i = 0;
        while (i < current->keys.size() && key >= current->keys[i])
        {
            i++;
        }
        current = current->children[i];
    }
    return current;
}

// deletes the tree recursively
void BPlusTree::clear(Node *node)
{
    if (node == nullptr)
    {
        return;
    }
    if (!node->isLeaf)
    {
        for (Node *child : node->children)
        {
            clear(child);
        }
    }
    delete node;
}

// helper function for inserting into leaf node
void BPlusTree::insertIntoLeaf(Node *leaf, const Record &record)
{
    auto it = std::lower_bound(leaf->records.begin(), leaf->records.end(), record);

    if (it != leaf->records.end() && it->date == record.date)
    {
        *it = record;
        return;
    }

    auto pos = it - leaf->records.begin();
    leaf->records.insert(it, record);
    leaf->keys.insert(leaf->keys.begin() + pos, record.date);
}

// helper function for when split is needed
std::pair<int, BPlusTree::Node *> BPlusTree::splitLeaf(Node *leaf)
{
    Node *rightL = new Node(true);
    const int splitIndex = static_cast<int>(leaf->records.size() + 1) / 2;
    rightL->records.assign(leaf->records.begin() + splitIndex, leaf->records.end());
    leaf->records.erase(leaf->records.begin() + splitIndex, leaf->records.end());
    rightL->keys.assign(leaf->keys.begin() + splitIndex, leaf->keys.end());
    leaf->keys.erase(leaf->keys.begin() + splitIndex, leaf->keys.end());
    rightL->next = leaf->next;
    leaf->next = rightL;
    const int movedKey = rightL->records.front().date;
    return {movedKey, rightL};
}

// helper function for inserting into internal after split
void BPlusTree::insertIntoInternal(Node *internal, int key, Node *rightChild)
{
    auto it = std::upper_bound(internal->keys.begin(), internal->keys.end(), key);
    std::size_t keyIndex = static_cast<std::size_t>(it - internal->keys.begin());

    internal->keys.insert(it, key);
    internal->children.insert(internal->children.begin() + static_cast<std::ptrdiff_t>(keyIndex + 1), rightChild);
}

// helper function for spliting after inserting into internal
std::pair<int, BPlusTree::Node *> BPlusTree::splitInternal(Node *internal)
{
    Node *rightI = new Node(false);
    const int splitIndex = static_cast<int>(internal->keys.size() + 1) / 2;
    rightI->keys.assign(internal->keys.begin() + splitIndex, internal->keys.end());
    internal->keys.erase(internal->keys.begin() + splitIndex, internal->keys.end());
    rightI->children.assign(internal->children.begin() + splitIndex, internal->children.end());
    internal->children.erase(internal->children.begin() + splitIndex, internal->children.end());
    const int movedKey = rightI->keys.front();
    return {movedKey, rightI};
}

// helper function for inserting tree recursively. True if you need to split false otherwise.
bool BPlusTree::insertRecursive(Node *node, const Record &record, int &movedKey, Node *&newRightChild)
{
    if (node->isLeaf)
    {
        insertIntoLeaf(node, record);

        if (static_cast<int>(node->records.size()) <= maxKeys())
        {
            return false;
        }
        std::pair<int, BPlusTree::Node *> splitResult = splitLeaf(node);
        movedKey = splitResult.first;
        newRightChild = splitResult.second;
        return true;
    }

    std::size_t iOfChild = 0;

    while (iOfChild < node->keys.size() && record.date >= node->keys[iOfChild])
    {
        iOfChild++;
    }

    int changedChildKey = 0;
    Node *childNewRight = nullptr;
    bool splitChild = insertRecursive(node->children[iOfChild], record, changedChildKey, childNewRight);
    if (!splitChild)
    {
        return false;
    }
    insertIntoInternal(node, changedChildKey, childNewRight);
    if (static_cast<int>(node->keys.size()) <= maxKeys())
    {
        return false;
    }

    std::pair<int, BPlusTree::Node *> splitResult = splitInternal(node);
    movedKey = splitResult.first;
    newRightChild = splitResult.second;
    return true;
}

BPlusTree::BPlusTree(int order) : root(nullptr), order(order)
{
    if (order < 3)
    {
        throw std::invalid_argument("B+ tree order must be at least 3");
    }
}

BPlusTree::BPlusTree(BPlusTree &&other) noexcept : root(other.root), order(other.order)
{
    other.root = nullptr;
}

BPlusTree &BPlusTree::operator=(BPlusTree &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }
    clear(root);
    root = other.root;
    order = other.order;
    other.root = nullptr;
    return *this;
}

BPlusTree::~BPlusTree()
{
    clear(root);
}

void BPlusTree::insert(const Record &record)
{
    if (root == nullptr)
    {
        root = new Node(true);
        root->records.push_back(record);
        root->keys.push_back(record.date);
        return;
    }
    int changedKey = 0;
    Node *newRightChild = nullptr;
    bool rootSplit = insertRecursive(root, record, changedKey, newRightChild);
    if (rootSplit)
    {
        Node *newRoot = new Node(false);
        newRoot->keys.push_back(changedKey);
        newRoot->children.push_back(root);
        newRoot->children.push_back(newRightChild);
        root = newRoot;
    }
}

Record *BPlusTree::search(int date)
{
    return const_cast<Record *>(static_cast<const BPlusTree &>(*this).search(date));
}

const Record *BPlusTree::search(int date) const
{
    Node *leaf = findLeaf(date);
    if (leaf == nullptr)
    {
        return nullptr;
    }

    auto it = std::lower_bound(
        leaf->records.begin(),
        leaf->records.end(),
        date,
        [](const Record &r, int date)
        {
            return r.date < date;
        });

    if (it != leaf->records.end() && it->date == date)
    {
        return &*it;
    }

    return nullptr;
}

bool BPlusTree::contains(int date) const
{
    return search(date) != nullptr;
}

std::vector<Record> BPlusTree::rangeQuery(int startDate, int endDate) const
{
    std::vector<Record> results;
    if (root == nullptr || startDate > endDate)
    {
        return results;
    }

    Node *current = findLeaf(startDate);
    while (current != nullptr)
    {
        for (const Record &record : current->records)
        {
            if (record.date < startDate)
            {
                continue;
            }
            if (record.date > endDate)
            {
                return results;
            }
            results.push_back(record);
        }
        current = current->next;
    }

    return results;
}
