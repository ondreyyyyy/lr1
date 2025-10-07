package main

import "fmt"

type QNode struct {
    value string
    next  *QNode
}

type Queue struct {
    head *QNode
    tail *QNode
}

func queueCreateNode(data string) *QNode {
    return &QNode{value: data, next: nil}
}

func createQueue() *Queue {
    return &Queue{head: nil, tail: nil}
}

func enqueue(queue *Queue, data string) {
    newNode := queueCreateNode(data)
    if queue.head == nil {
        queue.head = newNode
        queue.tail = newNode
        return
    }

    queue.tail.next = newNode
    queue.tail = newNode
}

func dequeue(queue *Queue) {
    if queue.head == nil {
        fmt.Println("Очередь пустая. Нечего удалять.")
        return
    }

    if queue.head == queue.tail {
        queue.head = nil
        queue.tail = nil
    } else {
        queue.head = queue.head.next
    }
}

func printQueue(queue *Queue) {
    if queue.head == nil {
        fmt.Println("Очередь пуста.")
        return
    }

    fmt.Print("Очередь: ")
    curElem := queue.head
    for curElem != nil {
        fmt.Print(curElem.value + " ")
        curElem = curElem.next
    }

    fmt.Println()
}
