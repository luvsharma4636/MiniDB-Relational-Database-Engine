#pragma once
using namespace std;

const int ORDER = 4;

struct BTreeNode {
    int keys[ORDER-1];
    int positions[ORDER-1];    // row position in .db file
    BTreeNode* children[ORDER];
    int num_keys;
    bool is_leaf;
    
    BTreeNode(bool leaf);
};

class BTree {
    BTreeNode* root;
    
    void insert_non_full(BTreeNode* node, int key, int pos);
    void split_child(BTreeNode* parent, int i, BTreeNode* child);
    int  search_node(BTreeNode* node, int key);
    void print_node(BTreeNode* node, int level);

public:
    BTree();
    void insert(int key, int position);
    int  search(int key);
    void print_tree();
};
