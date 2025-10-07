#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "stack.h"

using namespace std;

void createStack(Stack*& st) {
    st->head = nullptr;
}

SNode* createSNode(string data) { 
    SNode* newNode = new SNode{data, nullptr};
    return newNode;
}

void push(Stack* st, string data) {
    SNode* newNode = createSNode(data);
    newNode->next = st->head;
    st->head = newNode;
}

void pop(Stack* st) {
    if (st->head == nullptr) {
        cout << "Стек пуст.\n";
        return;
    }

    SNode* deleteNode = st->head;
    st->head = st->head->next;
    delete deleteNode;
}

void printStack(Stack* st) {
    if (st->head == nullptr) {
        cout << "Стек пуст.\n";
        return;
    }

    cout << "Стек: ";
    SNode* curElem = st->head;
    while (curElem != nullptr) {
        cout << curElem->value << " ";
        curElem = curElem->next;
    }

    cout << endl;
}