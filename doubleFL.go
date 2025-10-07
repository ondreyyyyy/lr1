package main

import "fmt"

type DNode struct {
    value string
    next  *DNode
    prev  *DNode
}

type DoubleFL struct {
    head *DNode
    tail *DNode
}

func createDoubleFL(beginNode string) DoubleFL {
    node := &DNode{value: beginNode}
    return DoubleFL{
        head: node,
        tail: node,
    }
}

func dblAddAtHead(doubleFlist *DoubleFL, data string) {
    if doubleFlist.head == nil {
        node := &DNode{value: data}
        doubleFlist.head = node
        doubleFlist.tail = node
        return
    }

    newHead := &DNode{value: data, next: doubleFlist.head}
    doubleFlist.head.prev = newHead
    doubleFlist.head = newHead
}

func dblAddAtTail(doubleFlist *DoubleFL, data string) {
    if doubleFlist.head == nil {
        node := &DNode{value: data}
        doubleFlist.head = node
        doubleFlist.tail = node
        return
    }

    newNode := &DNode{value: data, prev: doubleFlist.tail}
    doubleFlist.tail.next = newNode
    doubleFlist.tail = newNode
}

func dblAddBeforeIndex(doubleFlist *DoubleFL, data string, index int) {
    if doubleFlist.head == nil {
        fmt.Println("Список пуст.")
        return
    } else if index == 0 {
        dblAddAtHead(doubleFlist, data)
        return
    } else if index < 0 {
        fmt.Println("Неверный индекс!")
        return
    }

    i := 0
    curElem := doubleFlist.head

    for i != index && curElem != nil {
        curElem = curElem.next
        i++
    }

    if curElem == nil {
        fmt.Println("Элемента с таким индексом не существует!")
        return
    }

    newNode := &DNode{value: data, next: curElem, prev: curElem.prev}
    curElem.prev.next = newNode
    curElem.prev = newNode
}

func dblAddAfterIndex(doubleFlist *DoubleFL, data string, index int) {
    if doubleFlist.head == nil {
        fmt.Println("Список пуст.")
        return
    } else if index < 0 {
        fmt.Println("Неверный индекс!")
        return
    }

    curElem := doubleFlist.head
    i := 0

    for i != index && curElem != nil {
        curElem = curElem.next
        i++
    }

    if curElem == nil {
        fmt.Println("Элемента с таким индексом не существует!")
        return
    }

    if curElem == doubleFlist.tail {
        dblAddAtTail(doubleFlist, data)
        return
    }

    newNode := &DNode{value: data, next: curElem.next, prev: curElem}
    curElem.next.prev = newNode
    curElem.next = newNode
}

func dblDeleteAtHead(doubleFlist *DoubleFL) {
    if doubleFlist.head == nil {
        fmt.Println("Список пуст.")
        return
    }

    if doubleFlist.head == doubleFlist.tail {
        doubleFlist.head = nil
        doubleFlist.tail = nil
    } else {
        doubleFlist.head = doubleFlist.head.next
        doubleFlist.head.prev = nil
    }
}

func dblDeleteAtTail(doubleFlist *DoubleFL) {
    if doubleFlist.head == nil {
        fmt.Println("Список пуст.")
        return
    }

    if doubleFlist.head == doubleFlist.tail {
        doubleFlist.head = nil
        doubleFlist.tail = nil
    } else {
        doubleFlist.tail.prev.next = nil
        doubleFlist.tail = doubleFlist.tail.prev
    }
}

func dblDeleteBeforeIndex(doubleFlist *DoubleFL, index int) {
    if doubleFlist.head == nil || doubleFlist.head == doubleFlist.tail {
        fmt.Println("Удаление невозможно.")
        return
    } else if index == 1 {
        dblDeleteAtHead(doubleFlist)
        return
    } else if index <= 0 {
        fmt.Println("Неверный индекс.")
        return
    }

    curElem := doubleFlist.head
    i := 0

    for i != index && curElem != nil {
        curElem = curElem.next
        i++
    }

    if curElem == nil || curElem.prev == nil {
        fmt.Println("Неверный индекс.")
        return
    }

    deleteNode := curElem.prev

    if deleteNode == doubleFlist.head {
        dblDeleteAtHead(doubleFlist)
        return
    }

    deleteNode.prev.next = curElem
    curElem.prev = deleteNode.prev
}

func dblDeleteAfterIndex(doubleFlist *DoubleFL, index int) {
    if doubleFlist.head == nil || doubleFlist.head == doubleFlist.tail {
        fmt.Println("Удаление невозможно.")
        return
    } else if index < 0 {
        fmt.Println("Неверный индекс.")
        return
    }

    curElem := doubleFlist.head
    i := 0

    for i != index && curElem != nil {
        curElem = curElem.next
        i++
    }

    if curElem == nil || curElem == doubleFlist.tail {
        fmt.Println("Неверный индекс.")
        return
    }

    if curElem.next == doubleFlist.tail {
        dblDeleteAtTail(doubleFlist)
        return
    }

    deleteNode := curElem.next
    curElem.next = deleteNode.next
    deleteNode.next.prev = curElem
}

func dblDeleteValue(doubleFlist *DoubleFL, targetValue string) {
    if doubleFlist.head == nil {
        fmt.Println("Список пуст.")
        return
    }

    curElem := doubleFlist.head
    isFound := false

    for curElem != nil {
        if curElem.value == targetValue {
            isFound = true
            break
        }
        curElem = curElem.next
    }

    if !isFound {
        fmt.Println("Элемент не найден.")
        return
    }

    if curElem == doubleFlist.head {
        dblDeleteAtHead(doubleFlist)
        return
    }

    if curElem == doubleFlist.tail {
        dblDeleteAtTail(doubleFlist)
        return
    }

    deleteNode := curElem
    deleteNode.prev.next = deleteNode.next
    deleteNode.next.prev = deleteNode.prev
}

func dblSearchValue(doubleFlist *DoubleFL, targetValue string) {
    if doubleFlist.head == nil {
        fmt.Println("Список пуст.")
        return
    }

    curElem := doubleFlist.head
    isFound := false
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
    }

    fmt.Printf("Элемент %s найден. Индекс: %d\n", curElem.value, index)
}

func printDoubleFL(head *DNode) {
    fmt.Print("Двусвязный список: ")

    if head == nil {
        fmt.Println("Список пуст")
        return
    }

    for head != nil {
        fmt.Print(head.value + " ")
        head = head.next
    }

    fmt.Println()
}
