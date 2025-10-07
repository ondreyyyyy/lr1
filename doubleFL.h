#ifndef DOUBLEFL_H
#define DOUBLEFL_H

#include <string>

using namespace std;

struct DNode {
    string value;
    DNode* next;
    DNode* prev;
};

struct doubleFL {
    DNode* head;
    DNode* tail;
};

void createDoubleFL(string beginNode, doubleFL& doubleFlist);
void addAtHead(doubleFL& doubleFlist, string data);
void addAtTail(doubleFL& doubleFlist, string data);
void addBeforeIndex(doubleFL& doubleFlist, string data, int index);
void addAfterIndex(doubleFL& doubleFlist, string data, int index);
void deleteAtHead(doubleFL& doubleFlist);
void deleteAtTail(doubleFL& doubleFlist);
void deleteBeforeIndex(doubleFL& doubleFlist, int index);
void deleteAfterIndex(doubleFL& doubleFlist, int index);
void deleteValue(doubleFL& doubleFlist, string targetValue);
void searchValue(doubleFL& doubleFlist, string targetValue);
void printDoubleFL (DNode* doubleFlist);

#endif // DOUBLEFL_H