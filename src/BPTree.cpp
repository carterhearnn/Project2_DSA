#include "BPTree.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <stdexcept>

struct BPlusTree::Node{
    bool isLeaf;
    std::vector<int> keys;
    std::vector<Node*> children;
    std::vector<Record> records;
    Node* next;
    explicit Node(bool leaf = false) : isLeaf(leaf), next(nullptr) {}
};

int BPlusTree::maxKeys() const{
    return order - 1;
}

//finds leaf that has key recursively. 
BPlusTree::Node* BPlusTree::findLeaf(int key) const{
    if (root == nullptr){
        return nullptr;
    }
    Node* current = root;
    while (!current->isLeaf){
        std::size_t i = 0;
        while (i< current->keys.size() && key >= current->keys[i]){
            i++;
        }
        current = current->children[i];
    }
    return current;
}


//deletes the tree recursively
void BPlusTree::clear(Node* node){
    if (node == nullptr){
        return;
    }
    if (!node->isLeaf){
        for (Node* child : node->children){
            clear(child);
        }
    }
    delete node;
}



void BPlusTree::insertIntoLeaf(Node* leaf, const Record& record) {
    auto it = std::lower_bound(leaf->records.begin(), leaf->records.end(), record);

    if (it != leaf->records.end() && it->date == record.date) {
        *it = record;
        return;
    }

    auto pos = it - leaf->records.begin();
    leaf->records.insert(it, record);
    leaf->keys.insert(leaf->keys.begin() + pos, record.date);
}
