package main

import "fmt"

type SNode struct {
    value string
    next  *SNode
}

type Stack struct {
    head *SNode
}

func createStack() *Stack {
    return &Stack{head: nil}
}

func stackCreateSNode(data string) *SNode {
    return &SNode{value: data, next: nil}
}

func push(st *Stack, data string) {
    newNode := stackCreateSNode(data)
    newNode.next = st.head
    st.head = newNode
}

func pop(st *Stack) {
    if st.head == nil {
        fmt.Println("Стек пуст.")
        return
    }

    st.head = st.head.next
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
