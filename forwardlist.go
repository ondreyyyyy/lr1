package main

import "fmt"

type FNode struct {
    value string
    next  *FNode
}

type ForwardList struct {
    head *FNode
    tail *FNode
}

func flCreateNode(data string, next *FNode) *FNode {
    return &FNode{value: data, next: next}
}

func createFL(data string) ForwardList {
    firstNode := flCreateNode(data, nil)
    return ForwardList{
        head: firstNode,
        tail: firstNode,
    }
}

func flAddAtHead(fList *ForwardList, data string) {
    if fList.head == nil {
        *fList = createFL(data)
    } else {
        newHead := flCreateNode(data, fList.head)
        fList.head = newHead
    }
}

func flAddAtTail(fList *ForwardList, data string) {
    if fList.head == nil {
        *fList = createFL(data)
    } else {
        newNode := flCreateNode(data, nil)
        fList.tail.next = newNode
        fList.tail = newNode
    }
}

func flSearchNode(fList ForwardList, targetValue string) {
    if fList.head == nil {
        fmt.Println("Список пуст.")
        return
    }

    isFound := false
    curElem := fList.head
    index := 0

    for curElem != nil {
        if curElem.value == targetValue {
            isFound = true
            break
        }
        curElem = curElem.next
        index++
    }

    if !isFound {
        fmt.Println("Элемент не найден.")
        return
    } else {
        fmt.Printf("Элемент %s найден. Индекс: %d\n", targetValue, index)
        return
    }
}

func flAddBeforeIndex(fList *ForwardList, data string, index int) {
    if fList.head == nil {
        fmt.Println("Список пуст.")
        return
    } else if index == 0 {
        flAddAtHead(fList, data)
        return
    } else if index < 0 {
        fmt.Println("Неверный индекс!")
        return
    }

    i := 0
    curElem := fList.head

    for i != index-1 && curElem != nil {
        curElem = curElem.next
        i++
    }

    if curElem == nil {
        fmt.Println("Элемента с таким индексом не существует!")
        return
    }

    newNode := flCreateNode(data, curElem.next)
    curElem.next = newNode
}

func flAddAfterIndex(fList *ForwardList, data string, index int) {
    if fList.head == nil {
        fmt.Println("Список пуст.")
        return
    } else if index < 0 {
        fmt.Println("Неверный индекс!")
        return
    }

    curElem := fList.head
    i := 0

    for i != index && curElem != nil {
        curElem = curElem.next
        i++
    }

    if curElem == nil {
        fmt.Println("Элемента с таким индексом не существует!")
        return
    }

    newNode := flCreateNode(data, curElem.next)
    curElem.next = newNode

    if curElem == fList.tail {
        fList.tail = newNode
    }
}

func flDeleteAtHead(fList *ForwardList) {
    if fList.head == nil {
        fmt.Println("Нечего удалять.")
        return
    }

    fList.head = fList.head.next

    if fList.head == nil {
        fList.tail = nil
    }
}

func flDeleteAtTail(fList *ForwardList) {
    if fList.head == nil {
        fmt.Println("Список пуст.")
        return
    } else if fList.head == fList.tail {
        flDeleteAtHead(fList)
        return
    }

    curElem := fList.head

    for curElem.next != fList.tail {
        curElem = curElem.next
    }

    fList.tail = curElem
    fList.tail.next = nil
}

func flDeleteBeforeIndex(fList *ForwardList, index int) {
    if fList.head == nil || fList.head == fList.tail {
        fmt.Println("Удаление невозможно.")
        return
    } else if index == 1 {
        flDeleteAtHead(fList)
        return
    } else if index <= 0 {
        fmt.Println("Неверный индекс.")
        return
    }

    curElem := fList.head
    i := 0

    for i != index-2 && curElem != nil {
        curElem = curElem.next
        i++
    }

    if curElem == nil || curElem == fList.tail || curElem.next == nil {
        fmt.Println("Неверный индекс.")
        return
    }

    deleteNode := curElem.next
    curElem.next = deleteNode.next
}

func flDeleteAfterIndex(fList *ForwardList, index int) {
    if fList.head == nil || fList.head == fList.tail {
        fmt.Println("Удаление невозможно.")
        return
    } else if index < 0 {
        fmt.Println("Неверный индекс.")
        return
    }

    curElem := fList.head
    i := 0

    for i != index && curElem != nil {
        curElem = curElem.next
        i++
    }

    if curElem == nil || curElem == fList.tail {
        fmt.Println("Неверный индекс.")
        return
    }

    if curElem.next == fList.tail {
        flDeleteAtTail(fList)
        return
    }

    deleteNode := curElem.next
    curElem.next = deleteNode.next

    if deleteNode == fList.tail {
        fList.tail = curElem
    }
}

func flValueDelete(fList *ForwardList, data string) {
    if fList.head == nil {
        fmt.Println("Удаление невозможно.")
        return
    }

    curElem := fList.head
    var prevElem *FNode = nil
    isFound := false

    for curElem != nil {
        if curElem.value == data {
            isFound = true
            break
        }
        prevElem = curElem
        curElem = curElem.next
    }

    if !isFound {
        fmt.Println("Элемент не найден.")
        return
    }

    if curElem == fList.head {
        flDeleteAtHead(fList)
    } else if curElem == fList.tail {
        flDeleteAtTail(fList)
    } else {
        prevElem.next = curElem.next
    }
}

func printFL(head *FNode) {
    fmt.Print("Односвязный список: ")

    for head != nil {
        fmt.Print(head.value + " ")
        head = head.next
    }

    fmt.Println()
}
