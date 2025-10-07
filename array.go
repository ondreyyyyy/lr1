package main

type DynArray struct {
    data     []string
    size     int
    capacity int
}

func createArr(initialCapacity int) DynArray {
    if initialCapacity <= 0 {
        initialCapacity = 1
    }
    arr := DynArray{
        size:     0,
        capacity: initialCapacity,
        data:     make([]string, initialCapacity),
    }
    return arr
}

func addAtIndex(index int, data string, arr *DynArray) {
    if index < 0 || index > arr.size {
        return
    }

    if arr.size == arr.capacity {
        newCapacity := arr.capacity * 2
        newData := make([]string, newCapacity)
        
        for i := 0; i < index; i++ {
            newData[i] = arr.data[i]
        }
        
        newData[index] = data
        
        for i := index; i < arr.size; i++ {
            newData[i+1] = arr.data[i]
        }
        
        arr.data = newData
        arr.capacity = newCapacity
    } else {
        for i := arr.size; i > index; i-- {
            arr.data[i] = arr.data[i-1]
        }
        arr.data[index] = data
    }
    
    arr.size++
}

func pushBackArr(data string, arr *DynArray) {
    addAtIndex(arr.size, data, arr)
}

func getElementIndex(index int, arr DynArray) (string, error) {
    if index < 0 || index >= arr.size {
        return "", outOfRangeError("Индекс не является элементом массива.")
    }
    return arr.data[index], nil
}

func removeFromIndex(index int, arr *DynArray) {
    if index < 0 || index >= arr.size {
        return
    }

    for i := index; i < arr.size-1; i++ {
        arr.data[i] = arr.data[i+1]
    }
    
    arr.size--
}

func replacementElement(index int, data string, arr *DynArray) {
    if index < 0 || index >= arr.size {
        return
    }
    arr.data[index] = data
}

func genLength(arr DynArray) int {
    return arr.size
}

func printArray(arr DynArray) {
    if arr.size == 0 {
        println("Массив пуст.")
        return
    }

    for i := 0; i < arr.size; i++ {
        print(arr.data[i] + " ")
    }
    println()
}

type outOfRangeError string

func (e outOfRangeError) Error() string {
    return string(e)
}
