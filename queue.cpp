#include <iostream>
#include <string>
#include <fstream>
#include "queue.h"

using namespace std;

QNode* createNode(string data) {
    QNode* newNode = new QNode{data,nullptr};
    return newNode;
}

void createQueue(Queue* queue) {
    queue->head = nullptr;
    queue->tail = nullptr;
}

void enqueue(Queue* queue, string data) {
    QNode* newNode = createNode(data);
    if (queue->head == nullptr) {
        queue->head = newNode;
        queue->tail = newNode;
        return;
    }
    
    queue->tail->next = newNode;
    queue->tail = newNode;
}

void dequeue(Queue* queue) {
    if (queue->head == nullptr) {
        cout << "Очередь пустая. Нечего удалять.\n";
        return;
    }

    QNode* deleteNode = queue->head;
    if (deleteNode == queue->tail) {
        queue->head = nullptr;
        queue->tail = nullptr;
    }
    else {
        queue->head = queue->head->next;
    }
    delete deleteNode;
}

void printQueue(Queue* queue) {
    if (queue->head == nullptr) {
        cout << "Очередь пуста.\n";
        return;
    }

    cout << "Очередь: ";
    QNode* curElem = queue->head;
    while (curElem != nullptr) {
        cout << curElem->value << " ";
        curElem = curElem->next;
    }

    cout << endl;
}