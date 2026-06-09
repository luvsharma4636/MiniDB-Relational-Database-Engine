#include "../include/btree.h"
#include <iostream>
using namespace std;

BTreeNode::BTreeNode(bool leaf) {
    is_leaf = leaf;
    num_keys = 0;
    for(int i = 0; i < ORDER; i++)
        children[i] = nullptr;
}

BTree::BTree() {
    root = new BTreeNode(true);
}

int BTree::search_node(BTreeNode* node, int key) {
    int i = 0;
    while(i < node->num_keys && key > node->keys[i])
        i++;
    
    if(i < node->num_keys && key == node->keys[i])
        return node->positions[i];
    
    if(node->is_leaf) return -1;
    
    return search_node(node->children[i], key);
}

int BTree::search(int key) {
    return search_node(root, key);
}

void BTree::insert(int key, int position) {
    
    if(root->num_keys == ORDER - 1) {
        // Root is full — create new root
        BTreeNode* new_root = new BTreeNode(false);
        new_root->children[0] = root;
        split_child(new_root, 0, root);
        root = new_root;
    }
    insert_non_full(root, key, position);
}

void BTree::split_child(BTreeNode* parent, int i, BTreeNode* child) {
    int mid = (ORDER-1) / 2;
    BTreeNode* right = new BTreeNode(child->is_leaf);
    int right_keys = ORDER-mid-2;
    right->num_keys = right_keys;
    
    for(int j = 0; j < right_keys; j++) {
        right->keys[j] = child->keys[mid + 1 + j];
        right->positions[j] = child->positions[j + mid + 1];
    }
    
    if(!child->is_leaf) {
        for(int j = 0; j <=right_keys; j++)
            right->children[j] = child->children[mid+1+j];
    }
    
    child->num_keys = mid;
    
    for(int j = parent->num_keys; j > i ; j--)
        parent->children[j + 1] = parent->children[j];

    parent->children[i+1]=right;
    
   
    
    for(int j = parent->num_keys - 1; j >= i; j--) {
        parent->keys[j + 1] = parent->keys[j];
        parent->positions[j + 1] = parent->positions[j];
    }
    
    parent->keys[i] = child->keys[mid];
    parent->positions[i] = child->positions[mid];
    parent->num_keys++;
}

void BTree::insert_non_full(BTreeNode* node, int key, int pos) {
    int i = node->num_keys - 1;
    
    if(node->is_leaf) {
        while(i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            node->positions[i + 1] = node->positions[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->positions[i + 1] = pos;
        node->num_keys++;
    } else {
        while(i >= 0 && key < node->keys[i]) i--;
        i++;
        if(node->children[i]->num_keys == ORDER - 1) {
            split_child(node, i, node->children[i]);
            if(key > node->keys[i]) i++;
        }
        insert_non_full(node->children[i], key, pos);
    }
}