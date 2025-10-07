#include <iostream>
#include <string>
#include <fstream>
#include "doubleFL.h"

using namespace std;

void createDoubleFL(string beginNode, doubleFL& doubleFlist) {
    doubleFlist.head = new DNode{beginNode, nullptr, nullptr};
    doubleFlist.tail = doubleFlist.head;
}

void addAtHead(doubleFL& doubleFlist, string data) {
    if (doubleFlist.head == nullptr) {
        doubleFlist.head = new DNode{data, nullptr, nullptr};
        doubleFlist.tail = doubleFlist.head;
        return;
    }
    
    DNode* newHead = new DNode{data, doubleFlist.head, nullptr};
    doubleFlist.head->prev = newHead;
    doubleFlist.head = newHead;
}

void addAtTail(doubleFL& doubleFlist, string data) {
    if (doubleFlist.head == nullptr) {
        doubleFlist.head = new DNode{data, nullptr, nullptr};
        doubleFlist.tail = doubleFlist.head;
        return;
    }
    
    DNode* newNode = new DNode{data, nullptr, doubleFlist.tail};
    doubleFlist.tail->next = newNode;
    doubleFlist.tail = newNode;
}

void addBeforeIndex(doubleFL& doubleFlist, string data, int index) {
    if (doubleFlist.head == nullptr) {
        cout << "Список пуст.\n";
        return;
    }
    else if (index == 0) {
        addAtHead(doubleFlist, data);
        return;
    }
    else if (index < 0) {
        cout << "Неверный индекс!\n";
        return;
    }

    int i = 0;
    DNode* curElem = doubleFlist.head;

    for (; i != index && curElem != nullptr; i++) {
        curElem = curElem->next;
    }

    if (curElem == nullptr) {
        cout << "Элемента с таким индексом не существует!\n";
        return;
    }

    DNode* newNode = new DNode{data, curElem, curElem->prev};
    curElem->prev->next = newNode;
    curElem->prev = newNode;
}

void addAfterIndex(doubleFL& doubleFlist, string data, int index) {
    if (doubleFlist.head == nullptr) {
        cout << "Список пуст.\n";
        return;
    }
    else if (index < 0) {
        cout << "Неверный индекс!\n";
        return;
    }

    DNode* curElem = doubleFlist.head;
    int i = 0;

    for (; i != index && curElem != nullptr; i++) {
        curElem = curElem->next;
    }

    if (curElem == nullptr) {
        cout << "Элемента с таким индексом не существует!\n";
        return;
    }

    if (curElem == doubleFlist.tail) {
        addAtTail(doubleFlist, data);
        return;
    }

    DNode* newNode = new DNode{data, curElem->next, curElem};
    curElem->next->prev = newNode;
    curElem->next = newNode;
}

void deleteAtHead(doubleFL& doubleFlist) {
    if (doubleFlist.head == nullptr) {
        cout << "Список пуст.\n";
        return;
    }
    
    DNode* deleteNode = doubleFlist.head;

    if (doubleFlist.head == doubleFlist.tail) {
        doubleFlist.head = doubleFlist.tail = nullptr;
    }
    else {
        doubleFlist.head = doubleFlist.head->next;
        doubleFlist.head->prev = nullptr;
    }

    delete deleteNode;
}

void deleteAtTail(doubleFL& doubleFlist) {
    if (doubleFlist.head == nullptr) {
        cout << "Список пуст.\n";
        return;
    }
    
    DNode* deleteNode = doubleFlist.tail;

    if (doubleFlist.head == doubleFlist.tail) {
        doubleFlist.head = doubleFlist.tail = nullptr;
    }
    else {
        doubleFlist.tail->prev->next = nullptr;
        doubleFlist.tail = doubleFlist.tail->prev;
    }

    delete deleteNode;
}

void deleteBeforeIndex(doubleFL& doubleFlist, int index) {
    if (doubleFlist.head == nullptr || doubleFlist.head == doubleFlist.tail) {
        cout << "Удаление невозможно.\n";
        return;
    }
    else if (index == 1) {
        deleteAtHead(doubleFlist);
        return;
    }
    else if (index <= 0) {
        cout << "Неверный индекс.\n";
        return;
    }

    DNode* curElem = doubleFlist.head;
    int i = 0;

    for (; i != index && curElem != nullptr; i++) {
        curElem = curElem->next;
    }

    if (curElem == nullptr || curElem->prev == nullptr) {
        cout << "Неверный индекс.\n";
        return;
    }

    DNode* deleteNode = curElem->prev;
    
    if (deleteNode == doubleFlist.head) {
        deleteAtHead(doubleFlist);
        return;
    }

    deleteNode->prev->next = curElem;
    curElem->prev = deleteNode->prev;
    delete deleteNode;
}

void deleteAfterIndex(doubleFL& doubleFlist, int index) {
    if (doubleFlist.head == nullptr || doubleFlist.head == doubleFlist.tail) {
        cout << "Удаление невозможно.\n";
        return;
    }
    else if (index < 0) {
        cout << "Неверный индекс.\n";
        return;
    }
    
    DNode* curElem = doubleFlist.head;
    int i = 0;

    for (; i != index && curElem != nullptr; i++) {
        curElem = curElem->next;
    }

    if (curElem == nullptr || curElem == doubleFlist.tail) {
        cout << "Неверный индекс.\n";
        return;
    }

    if (curElem->next == doubleFlist.tail) {
        deleteAtTail(doubleFlist);
        return;
    }

    DNode* deleteNode = curElem->next;
    curElem->next = deleteNode->next;
    deleteNode->next->prev = curElem;
    delete deleteNode;
}

void deleteValue(doubleFL& doubleFlist, string targetValue) {
    if (doubleFlist.head == nullptr) {
        cout << "Список пуст.\n";
        return;
    }

    DNode* curElem = doubleFlist.head;
    bool isFound = false;

    while (curElem != nullptr) {
        if (curElem->value == targetValue) {
            isFound = true;
            break;
        }

        curElem = curElem->next;
    }

    if (!isFound) {
        cout << "Элемент не найден.\n";
        return;
    }

    if (curElem == doubleFlist.head) {
        deleteAtHead(doubleFlist);
        return;
    }

    if (curElem == doubleFlist.tail) {
        deleteAtTail(doubleFlist);
        return;
    }

    DNode* deleteNode = curElem;
    deleteNode->prev->next = deleteNode->next;
    deleteNode->next->prev = deleteNode->prev;
    delete deleteNode;
}

void searchValue(doubleFL& doubleFlist, string targetValue) {
    if (doubleFlist.head == nullptr) {
        cout << "Список пуст.\n";
        return;
    }

    DNode* curElem = doubleFlist.head;
    bool isFound = false;
    int index = 0;

    while (curElem != nullptr) {
        if (curElem->value == targetValue) {
            isFound = true;
            break;
        }

        curElem = curElem->next;
        index++;
    }

    if (!isFound) {
        cout << "Элемент не найден.\n";
        return;
    }
    
    cout << "Элемент " << curElem->value << " найден. Индекс: " << index << endl;
}

void printDoubleFL (DNode* doubleFlist) {
    cout << "Двусвязный список: ";

    if (doubleFlist == nullptr) {
        cout << "Список пуст\n";
        return;
    }

    while (doubleFlist != nullptr) {
        cout << doubleFlist->value << " ";
        doubleFlist = doubleFlist->next;
    }

    cout << endl;
}