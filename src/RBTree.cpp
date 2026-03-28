#include "RBTree.h"

// Constructor
RBTree::RBTree() : root(nullptr), nodeCount(0) {}

// Destructor
RBTree::~RBTree()
{
    destroyTree(root);
}

void RBTree::destroyTree(RBNode *node)
{
    if (node == nullptr)
        return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

// ---------
// Rotations
// ---------

void RBTree::rotateLeft(RBNode *node)
{
    RBNode *rightChild = node->right;

    // Move right childs left subtree to the right of the node
    node->right = rightChild->left;
    if (rightChild->left != nullptr)
    {
        rightChild->left->parent = node;
    }

    // Attach right child to parent of node
    rightChild->parent = node->parent;
    if (node->parent == nullptr)
    {
        root = rightChild;
    }
    else if (node == node->parent->left)
    {
        node->parent->left = rightChild;
    }
    else
    {
        node->parent->right = rightChild;
    }

    // Place node to the left of the right Child
    rightChild->left = node;
    node->parent = rightChild;
}

void RBTree::rotateRight(RBNode *node)
{
    RBNode *leftChild = node->left;

    // Move left childs right subtree to the left of the node
    node->left = leftChild->right;
    if (leftChild->right != nullptr)
    {
        leftChild->right->parent = node;
    }

    // Attach left child to parent of node
    leftChild->parent = node->parent;
    if (node->parent == nullptr)
    {
        root = leftChild;
    }
    else if (node == node->parent->right)
    {
        node->parent->right = leftChild;
    }
    else
    {
        node->parent->left = leftChild;
    }

    // Place node to the right of the Left Child
    leftChild->right = node;
    node->parent = leftChild;
}

// Insertion Function
void RBTree::insert(const Record &record)
{
    RBNode *newNode = new RBNode(record);

    // BST Insert
    RBNode *parent = nullptr;
    RBNode *current = root;

    while (current != nullptr)
    {
        parent = current;
        if (record < current->data)
        {
            current = current->left;
        }
        else if (record > current->data)
        {
            current = current->right;
        }
        else
        {
            // Duplicated date -> update record
            current->data = record;
            delete newNode;
            return;
        }
    }
    // Attached new node to parent
    newNode->parent = parent;
    if (parent == nullptr)
    {
        root = newNode;
    }
    else if (record < parent->data)
    {
        parent->left = newNode;
    }
    else
    {
        parent->right = newNode;
    }
    nodeCount++;

    // Fix any errors using fixup function
    insertFixup(newNode);
}

// Insertion Fixup Function
void RBTree::insertFixup(RBNode *node)
{
    while (node != root && node->parent->color == RED)
    {
        RBNode *grandparent = node->parent->parent;

        // Parent is Left Child of Grandparent
        if (node->parent == grandparent->left)
        {
            RBNode *uncle = grandparent->right;

            // Case1: Uncle is RED (recolor)
            if (uncle != nullptr && uncle->color == RED)
            {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            }
            else
            {
                // Case2: Node is right child -> rotate left
                if (node == node->parent->right)
                {
                    node = node->parent;
                    rotateLeft(node);
                }
                // Case3: Node is left child -> rotate grandparent right
                node->parent->color = BLACK;
                grandparent->color = RED;
                rotateRight(grandparent);
            }
        }
        // Parent is Right Child of grandparent
        else
        {
            RBNode *uncle = grandparent->left;

            // Case1: Uncle is RED (recolor)
            if (uncle != nullptr && uncle->color == RED)
            {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            }
            else
            {
                // Case2: Node is left child -> rotate right
                if (node == node->parent->left)
                {
                    node = node->parent;
                    rotateRight(node);
                }
                // Case3: Node is right child -> rotate grandparent left
                node->parent->color = BLACK;
                grandparent->color = RED;
                rotateLeft(grandparent);
            }
        }
    }
    // Root is always Black
    root->color = BLACK;
}

// Search Function
RBNode *RBTree::search(int date) const
{
    RBNode *current = root;

    while (current != nullptr)
    {
        if (date < current->data.date)
        {
            current = current->left;
        }
        else if (date > current->data.date)
        {
            current = current->right;
        }
        else
        {
            return current;
        }
    }
    return nullptr;
}

// Range Query Function
std::vector<Record> RBTree::rangeQuery(int startDate, int endDate) const
{
    std::vector<Record> results;
    rangeQueryHelper(root, startDate, endDate, results);
    return results;
}
void RBTree::rangeQueryHelper(RBNode *node, int startDate, int endDate, std::vector<Record> &results) const
{
    if (node == nullptr)
        return;

    // Go left if there are dates >= startDate
    if (node->data.date > startDate)
    {
        rangeQueryHelper(node->left, startDate, endDate, results);
    }

    // Include node if it falls within initialized range
    if (node->data.date >= startDate && node->data.date <= endDate)
    {
        results.push_back(node->data);
    }

    // Go right if there are dates <= endDate
    if (node->data.date < endDate)
    {
        rangeQueryHelper(node->right, startDate, endDate, results);
    }
}

// Other Functions
int RBTree::size() const
{
    return nodeCount;
}
bool RBTree::isEmpty() const
{
    return root == nullptr;
}