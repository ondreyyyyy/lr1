#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <string>

using namespace std;

struct QNode {
    string value;
    QNode* next;
};

struct Queue {
    QNode* head;
    QNode* tail;
};

QNode* createNode(string data);
void createQueue(Queue* queue);
void enqueue(Queue* queue, string data);
void dequeue(Queue* queue);
void printQueue(Queue* queue);

#endif // QUEUE_H