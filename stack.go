package main

import (
	"errors"
	"fmt"
)

type StackNode struct {
    value string
    next *StackNode
}

type Stack struct {
    head *StackNode
}

func createStack() *Stack {
    return &Stack{
        head: nil,
    }
}

func destroyStack(stack *Stack) {
    for stack.head != nil {
        //temp := stack.head
        stack.head = stack.head.next
        //temp = nil
    }
    stack = nil
}

func push(stack *Stack, value string) {
    newNode := &StackNode{
        value: value,
        next: stack.head,
    }
    stack.head = newNode
}

func pop(stack *Stack) (string, error) {
    if stack.head == nil {
        return "", errors.New("error")
    }
    
    futureDelete := stack.head
    value := futureDelete.value
    stack.head = stack.head.next
    futureDelete = nil
    return value, nil
}

func getStackElement(stack *Stack, index int) (string, error) {
    if index < 0 {
        return "", errors.New("error")
    }
    
    current := stack.head
    for i := 0; i < index; i++ {
        if current == nil {
            return "", errors.New("error")
        }
        current = current.next
    }
    
    if current == nil {
        return "", errors.New("error")
    }
    return current.value, nil
}

func printStack(st *Stack) {
    if st.head == nil {
        fmt.Println("Стек пуст.")
        return
    }

    fmt.Print("Стек: ")
    curElem := st.head
    for curElem != nil {
        fmt.Print(curElem.value + " ")
        curElem = curElem.next
    }

    fmt.Println()
}
