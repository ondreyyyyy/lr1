#include <iostream>
#include "tree.h"

using namespace std;

TNode* createNode(int key, Color color, TNode* parent, Tree* RBT) {
    return new TNode{key, color, RBT->nil, RBT->nil, parent};
}

Tree* createTree() {
    Tree* newTree = new Tree;
    newTree->nil = new TNode;
    newTree->nil->color = BLACK;
    newTree->nil->parent = newTree->nil;
    newTree->nil->left = newTree->nil;
    newTree->nil->right = newTree->nil;
    newTree->root = newTree->nil;
    return newTree;
}

void removeSubTree(Tree* RBT, TNode* node) {
    if (node == RBT->nil) {
        return;
    }

    removeSubTree(RBT, node->left);
    removeSubTree(RBT, node->right);
    delete node;
}

void removeTree(Tree* RBT) {
    removeSubTree(RBT, RBT->root);
    delete RBT->nil;
    delete RBT;
}

void leftRotate(Tree* RBT, TNode* x) {
    TNode* y = x->right; // перенос левого поддерева y к x
    x->right = y->left;
    if (y->left != RBT->nil) {
        y->left->parent = x;
    }

    y->parent = x->parent; // обновление указателей родителя или корня
    if (x->parent == RBT->nil) {
        RBT->root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }

    y->left = x; // x дочерний левый узел x
    x->parent = y; // родитель x - это y
}

void rightRotate(Tree* RBT, TNode* y) {
    TNode* x = y->left;
    y->left = x->right;
    if (x->right != RBT->nil) {
        x->right->parent = y;
    }

    x->parent = y->parent;
    if (y->parent == RBT->nil) {
        RBT->root = x;
    }
    else if (y == y->parent->left) {
        y->parent->left = x;
    }
    else {
        y->parent->right = x;
    }

    x->right = y;
    y->parent = x;
}

void insertFixup(Tree* RBT, TNode* z) { // восстановление свойств КЧД
    while (z->parent->color == RED) { // восстановление свойства 4
        if (z->parent == z->parent->parent->left) { // родитель - левый потомок дедушки
            TNode* y = z->parent->parent->right; // указатель на дядю
            if (y->color == RED) { // случай 1 - дядя красный
                y->color = BLACK;
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else { // дядя черный
                if (z == z->parent->right) { // случай 2 - правый потомок
                    z = z->parent;
                    leftRotate(RBT, z);
                }

                z->parent->color = BLACK; // случай 3 - левый потомок
                z->parent->parent->color = RED;
                rightRotate(RBT,z->parent->parent);
            }
        }
        else { // родитель - правый потомок дедушки
            TNode* y = z->parent->parent->left; // указатель на дядю
            if (y->color == RED) { // случай 1 - дядя красный
                y->color = BLACK;
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else { // дядя черный
                if (z == z->parent->left) { // случай 2 - левый потомок
                    z = z->parent;
                    rightRotate(RBT,z);
                }

                z->parent->color = BLACK; // случай 3 - правый потомок
                z->parent->parent->color = RED;
                leftRotate(RBT, z->parent->parent);
            }
        }
    }
    RBT->root->color = BLACK; // восстановление свойства 2 
}

void insert(Tree* RBT, int value) {
    TNode* z = createNode(value, RED, RBT->nil, RBT); // создание узла

    TNode* y = RBT->nil; // проход по дереву
    TNode* x = RBT->root;
    while (x != RBT->nil) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        }
        else if (z->key > x->key) {
            x = x->right;
        }
        else {
            delete z;
            return;
        }
    }

    z->parent = y; // вставка в дерево
    if (y == RBT->nil) {
        RBT->root = z;
    }
    else if (z->key < y->key) {
        y->left = z;
    }
    else {
        y->right = z;
    }

    insertFixup(RBT, z); // восстановление свойств КЧД
}

TNode* findNode(Tree* RBT, int value) { // поиск по значению
    TNode* currentNode = RBT->root;

    while (currentNode != RBT->nil && currentNode->key != value) {
        if (value < currentNode->key) {
            currentNode = currentNode->left;
        }
        else {
            currentNode = currentNode->right;
        }
    } 

    return currentNode;
}

void deleteFixup(Tree* RBT, TNode* x) {
    while (x != RBT->root && x->color == BLACK) {
        if (x == x->parent->left) { // случай 1 - левый потомок 
            TNode* w = x->parent->right; // брат

            if (w->color == RED) { // 1.1 - брат красный -> 1.2 || 1.3 || 1.4
                w->color = BLACK;
                w->parent->color = RED;
                leftRotate(RBT,x->parent);
                w = x->parent->right;
            }
            
            if (w->left->color == BLACK && w->right->color == BLACK) { // 1.2 - оба потомка брата черные
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->right->color == BLACK) { // 1.3 - правый потомок брата черный -> 1.4
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(RBT, w);
                    w = x->parent->right;
                }
                
                w->color = x->parent->color;// 1.4 - правый потомок брата красный
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(RBT, x->parent);
                x = RBT->root;
            }
        }
        else { // случай 2 - правый потомок
            TNode* w = x->parent->left; // брат

            if (w->color == RED) { // 2.1 - брат красный -> 2.2 || 2.3 || 2.4
                w->color = BLACK;
                w->parent->color = RED;
                rightRotate(RBT,x->parent);
                w = x->parent->left;
            }
            
            if (w->right->color == BLACK && w->left->color == BLACK) { // 2.2 - оба потомка брата черные
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->left->color == BLACK) { // 2.3 - левый потомок брата черный -> 2.4
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(RBT, w);
                    w = x->parent->left;
                }
                
                w->color = x->parent->color;// 2.4 - левый потомок брата красный
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(RBT, x->parent);
                x = RBT->root;
            }   
        }
    }
    x->color = BLACK;
}

void deleteNode(Tree* RBT, int key) {
    TNode* z = findNode(RBT, key);
    if (z == RBT->nil) {
        throw runtime_error("Ошибка: узел не найден.\n");
    }

    TNode* y = z; // удаляемый узел
    TNode* x; // тот, что встанет на его место
    Color colorY = y->color;

    if (z->right == RBT->nil) { // случай 1.1 - нет левого потомка (+ случай 2 - нет потомков вообще)
        x = z->left;

        if (z->parent == RBT->nil) { // 1.1.1 - удаляемый узел корень
            RBT->root = x;
        }
        else if (z == z->parent->left) { // 1.1.2 - удаляемый узел левый потомок
            z->parent->left = x;
        }
        else { // 1.1.3 - удаляемый узел правый потомок
            z->parent->right = x;
        }
        x->parent = z->parent; // у заменяющего узла новый родитель
        delete z;
    }
    else if (z->left == RBT->nil) { // случай 1.2 - нет левого потомка
        x = z->right;

        if (z->parent == RBT->nil) { // 1.2.1 - корень
            RBT->root = x;
        }
        else if (z == z->parent->left) { // 1.2.2 - левый потомок
            z->parent->left = x;
        }
        else { // 1.2.3 - правый потомок
            z->parent->right = x;
        }
        x->parent = z->parent;
        delete z;
    }
    else { // случай 3 - у удаляемого узла оба потомка
        y = z->left;
        while (y->right != RBT->nil) {
            y=y->right;
        }
        colorY = y->color;
        x = y->left;

        // замена y на x
        if (y->parent == z) { // 3.1 - родитель преемника - это z
            x->parent = y;
        }
        else { // 3.2 - иначе
            if (y->parent == RBT->nil) { 
                RBT->root = x;
            }
            else if (y == y->parent->left) { 
                y->parent->left = x;
            }
            else { 
                y->parent->right = x;
            }
            x->parent = y->parent;

            y->left = z->left;
            y->left->parent = y; // перенос правого поддерева к y
        }
        
        // замена z на y
        if (z->parent == RBT->nil) {
            RBT->root = y;
        }
        else if (z == z->parent->left) { 
            z->parent->left = y;
        }
        else { 
            z->parent->right = y;
        }
        y->parent = z->parent;

        y->right = z->right;
        y->right->parent = y;
        y->color = z->color;
        delete z;
    }

    if (colorY == BLACK) { // восстановление свойств
        deleteFixup(RBT, x);
    }
}

void printTreeHelper(TNode* node, Tree* RBT, string prefix, bool isLeft, bool isRoot) {
    if (node == RBT->nil) {
        return;
    }

    cout << prefix;
    
    if (isRoot) {
        cout << "└── ";
    } else {
        cout << (isLeft ? "├── " : "└── ");
    }
    
    string color_str = (node->color == RED) ? "R" : "B";
    cout << node->key << "(" << color_str << ")" << endl;
    
    string newPrefix = prefix + (isRoot ? "    " : (isLeft ? "│   " : "    "));

    bool hasRight = (node->right != RBT->nil);
    bool hasLeft = (node->left != RBT->nil);
    
    if (hasRight) {
        printTreeHelper(node->right, RBT, newPrefix, true, false);
    }
    
    if (hasLeft) {
        printTreeHelper(node->left, RBT, newPrefix, false, false);
    }

    if (!hasRight && !hasLeft) {
        cout << newPrefix;
        if (isRoot) {
            cout << "└── ";
        } else {
            cout << (isLeft ? "├── " : "└── ");
        }
        cout << "nil" << endl;
    }
}

void printTree(Tree* RBT) {
    if (RBT->root == RBT->nil) {
        cout << "Дерево пустое" << endl;
        return;
    }
    cout << "Красно-черное дерево:" << endl;
    printTreeHelper(RBT->root, RBT, "", false, true);
}

void saveTreeHelper(TNode* node, Tree* RBT, ostream& os) {
    if (node == RBT->nil) {
        os << "N ";
        return;
    }
    
    os << node->key << " " << (node->color == RED ? "R " : "B ");
    saveTreeHelper(node->left, RBT, os);
    saveTreeHelper(node->right, RBT, os);
}

void saveTreeToStream(Tree* RBT, ostream& os) {
    saveTreeHelper(RBT->root, RBT, os);
}

void loadTreeHelper(Tree* RBT, TNode*& node, TNode* parent, istream& is) {
    string token;
    if (!(is >> token)) return;
    
    if (token == "N") {
        node = RBT->nil;
        return;
    }
    
    int key = stoi(token);
    string color;
    is >> color;
    
    node = createNode(key, (color == "R" ? RED : BLACK), parent, RBT);
    loadTreeHelper(RBT, node->left, node, is);
    loadTreeHelper(RBT, node->right, node, is);
}

void loadTreeFromStream(Tree* RBT, istream& is) {
    loadTreeHelper(RBT, RBT->root, RBT->nil, is);
}