#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <stdexcept>

using namespace std;

enum Color {
    RED,
    BLACK
};

struct TNode {
    int key;
    Color color;
    TNode* left;
    TNode* right;
    TNode* parent;
};

struct Tree {
    TNode* root;
    TNode* nil;
};

Tree* createTree();
void removeTree(Tree* RBT);
void insert(Tree* RBT, int value);
void deleteNode(Tree* RBT, int key);
TNode* findNode(Tree* RBT, int value);
void printTree(Tree* RBT);
void saveTreeToStream(Tree* RBT, ostream& os);
void loadTreeFromStream(Tree* RBT, istream& is);

#endif