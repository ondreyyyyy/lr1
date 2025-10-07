#include <iostream>
#include <string>
#include <fstream>
#include "forwardlist.h"

using namespace std;

FNode* createNode(string data, FNode* next) {
    return new FNode{data, next};
}

void createFL(forwardList& fList, string data) { 
    FNode* firstNode = createNode(data, nullptr);
    fList.head = firstNode;
    fList.tail = firstNode;
}

void addAtHead(forwardList& fList, string data) {
    if (fList.head == nullptr) {
        createFL(fList, data);
    }
    else {
        FNode* newHead = createNode(data, fList.head);
        fList.head = newHead;
    }
}

void addAtTail(forwardList& fList, string data) {
    if (fList.head == nullptr) {
        createFL(fList, data);
    }
    else {
        FNode* newNode = createNode(data, nullptr);
        fList.tail -> next = newNode;
        fList.tail = newNode;
    }
}

void searchNode(const forwardList& fList, string targetValue) {
    if (fList.head == nullptr) {
        cout << "Список пуст.\n";
        return;
    }
    
    bool isFound = false;
    FNode* curElem = fList.head;
    int index = 0;

    while (curElem != nullptr) {
        if (curElem -> value == targetValue) {
            isFound = true;
            break;
        }
            
        curElem = curElem -> next;
        index++;
    }

    if (!isFound) {
        cout << "Элемент не найден.\n";
        return;
    }
    else {
        cout << "Элемент " << targetValue << " найден. Индекс: " << index << endl;
        return;
    }
}

void addBeforeIndex(forwardList& fList, string data, int index) {
    if (fList.head == nullptr) {
        cout << "Список пуст.";
        return;
    }
    else if (index == 0) {
        addAtHead(fList, data);
        return;
    }
    else if (index < 0) {
        cout << "Неверный индекс!\n";
        return;
    }

    int i = 0;
    FNode* curElem = fList.head;

    for (; i != index - 1 && curElem != nullptr; i++) {
        curElem  = curElem->next;
    }

    if (curElem == nullptr) {
        cout << "Элемента с таким индексом не существует!\n";
        return;
    }

    FNode* newNode = createNode(data, curElem->next);
    curElem->next = newNode;
}

void addAfterIndex(forwardList& fList, string data, int index) {
    if (fList.head == nullptr) {
        cout << "Список пуст.";
        return;
    }
    else if (index < 0) {
        cout << "Неверный индекс!\n";
        return;
    }

    FNode* curElem = fList.head;
    int i = 0;

    for (; i != index && curElem != nullptr; i++) {
        curElem = curElem->next;
    }

    if (curElem == nullptr) {
        cout << "Элемента с таким индексом не существует!\n";
        return;
    }

    FNode* newNode = createNode(data, curElem->next);
    curElem->next = newNode;

    if (curElem == fList.tail) {
        fList.tail = newNode;
    }
}

void deleteAtHead (forwardList& fList) {
    if (fList.head == nullptr) {
        cout << "Нечего удалять.\n";
        return;
    }
    
    FNode* deleteNode = fList.head;
    fList.head = fList.head -> next;

    if (fList.head == nullptr) {
        fList.tail = nullptr;
    }

    delete deleteNode;
}

void deleteAtTail(forwardList& fList) {
    if (fList.head == nullptr) {
        cout << "Список пуст.\n";
        return;
    }
    else if (fList.head == fList.tail) {
        deleteAtHead(fList);
        return;
    }
    
    FNode* curElem = fList.head;
    
    while (curElem -> next != fList.tail) {
        curElem = curElem -> next;
    }
    
    delete fList.tail;
    fList.tail = curElem;
    fList.tail -> next = nullptr;
}

void deleteBeforeIndex(forwardList& fList, int index) {
    if (fList.head == nullptr || fList.head == fList.tail) {
        cout << "Удаление невозможно.\n";
        return;
    }
    else if (index == 1) {
        deleteAtHead(fList);
        return;
    }
    else if (index <= 0) {
        cout << "Неверный индекс.\n";
        return;
    }

    FNode* curElem = fList.head;
    int i = 0;

    for (; i != index - 2 && curElem != nullptr; i++) {
        curElem = curElem->next;
    }

    if (curElem == nullptr || curElem == fList.tail || curElem->next == nullptr) {
        cout << "Неверный индекс.\n";
        return;
    }
    
    FNode* deleteNode = curElem->next;
    curElem->next = deleteNode->next; 
    delete deleteNode;
}

void deleteAfterIndex(forwardList& fList, int index) {
    if (fList.head == nullptr || fList.head == fList.tail) {
        cout << "Удаление невозможно.\n";
        return;
    }
    else if (index < 0) {
        cout << "Неверный индекс.\n";
        return;
    }
    
    FNode* curElem = fList.head;
    int i = 0;

    for (; i != index && curElem != nullptr; i++) {
        curElem = curElem->next;
    }

    if (curElem == nullptr || curElem == fList.tail) {
        cout << "Неверный индекс.\n";
        return;
    }

    if (curElem -> next == fList.tail) {
        deleteAtTail(fList);
        return;
    }

    FNode* deleteNode = curElem -> next;
    curElem -> next = deleteNode -> next;

    if (deleteNode == fList.tail) {
        fList.tail = curElem;
    }

    delete deleteNode;
}

void valueDelete(forwardList& fList, string data) {
    if (fList.head == nullptr) {
        cout << "Удаление невозможно.\n";
        return;
    } 

    FNode* curElem = fList.head;
    FNode* prevElem = nullptr;
    bool isFound = false;
    
    while (curElem != nullptr) {
        if (curElem->value == data) {
            isFound = true;
            break;
        }

        prevElem = curElem;
        curElem = curElem -> next;
    }

    if (!isFound) {
        cout << "Элемент не найден.\n";
        return;
    }

    if (curElem == fList.head) {
        deleteAtHead(fList);
    }
    else if (curElem == fList.tail) {
        deleteAtTail(fList);
    }
    else {
        prevElem->next = curElem->next;
        delete curElem;
    }
}

void printFL(FNode* fList) {
    cout << "Односвязный список: ";

    while (fList != nullptr) {
        cout << fList -> value << " ";
        fList = fList -> next;
    }

    cout << endl;
}