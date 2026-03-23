#include <iostream>
#include <vector> 


class BPlusTree {
    private:
        struct Node {
            std::vector<int> keys;
            std::vector<Node*> children;
            bool isLeaf;
        };

        Node* root;
        int order;
};