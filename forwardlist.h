#ifndef FORWARDLIST_H
#define FORWARDLIST_H

#include <string>

using namespace std;

struct FNode {
    string value;
    FNode* next;
};

struct forwardList {
    FNode* head;
    FNode* tail;
};

FNode* createNode(string data, FNode* next);
void createFL(forwardList& fList, string data);
void addAtHead(forwardList& fList, string data);
void addAtTail(forwardList& fList, string data);
void searchNode(const forwardList& fList, string targetValue);
void addBeforeIndex(forwardList& fList, string data, int index);
void addAfterIndex(forwardList& fList, string data, int index);
void deleteAtHead (forwardList& fList);
void deleteAtTail(forwardList& fList);
void deleteBeforeIndex(forwardList& fList, int index);
void deleteAfterIndex(forwardList& fList, int index);
void valueDelete(forwardList& fList, string data);
void printFL(FNode* fList);

#endif // FORWARDLIST_H