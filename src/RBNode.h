#ifndef RBNODE_H
#define RBNODE_H

#include "Record.h"

enum Color
{
    RED,
    BLACK
};

struct RBNode
{
    Record data;
    Color color;
    RBNode *left;
    RBNode *right;
    RBNode *parent;

    // Constructor (new nodes always red when inserted)
    RBNode(const Record &record) : data(record), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

#endif