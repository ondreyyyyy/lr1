#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <string>

using namespace std;

struct SNode {
    string value;
    SNode* next;
};

struct Stack {
    SNode* head;
};

void createStack(Stack*& st);
SNode* createSNode(string data); 
void push(Stack* st, string data);
void pop(Stack* st);
void printStack(Stack* st);

#endif // STACK_H