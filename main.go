package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

var arrays = make(map[string]*DynArray)
var forwardLists = make(map[string]*ForwardList)
var doubleLists = make(map[string]*DoubleFL)
var stacks = make(map[string]*Stack)
var queues = make(map[string]*Queue)

func saveToFile(filename string) {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка при создании файла: %v\n", err)
		return
	}
	defer file.Close()

	writer := bufio.NewWriter(file)

	for name, arr := range arrays {
		writer.WriteString("ARRAY " + name)
		for i := 0; i < arr.size; i++ {
			elem, err := getElementIndex(i, *arr)
			if err == nil {
				writer.WriteString(" " + elem)
			}
		}
		writer.WriteString("\n")
	}

	for name, list := range forwardLists {
		writer.WriteString("FORWARDLIST " + name)
		current := list.head
		for current != nil {
			writer.WriteString(" " + current.value)
			current = current.next
		}
		writer.WriteString("\n")
	}

	for name, list := range doubleLists {
		writer.WriteString("DOUBLELIST " + name)
		current := list.head
		for current != nil {
			writer.WriteString(" " + current.value)
			current = current.next
		}
		writer.WriteString("\n")
	}

	for name, stack := range stacks {
		writer.WriteString("STACK " + name)
		// Создаем временный стек для правильного порядка записи
		//temp := createStack()
		current := stack.head
		// Копируем элементы в обратном порядке
		elements := []string{}
		for current != nil {
			elements = append([]string{current.value}, elements...)
			current = current.next
		}
		// Записываем элементы в правильном порядке (от дна к вершине)
		for _, elem := range elements {
			writer.WriteString(" " + elem)
		}
		writer.WriteString("\n")
	}

	for name, queue := range queues {
		writer.WriteString("QUEUE " + name)
		current := queue.head
		for current != nil {
			writer.WriteString(" " + current.value)
			current = current.next
		}
		writer.WriteString("\n")
	}

	writer.Flush()
}

func loadFromFile(filename string) {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("Ошибка при открытии файла: %v\n", err)
		return
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		if line == "" {
			continue
		}

		tokens := strings.Fields(line)
		if len(tokens) < 2 {
			continue
		}

		typeStr := tokens[0]
		name := tokens[1]

		switch typeStr {
		case "ARRAY":
			capacity := len(tokens) - 2
			if capacity <= 0 {
				capacity = 1
			}
			arr := createArr(capacity)
			for i := 2; i < len(tokens); i++ {
				pushBackArr(tokens[i], &arr)
			}
			arrays[name] = &arr

		case "FORWARDLIST":
			if len(tokens) > 2 {
				list := createFL(tokens[2])
				for i := 3; i < len(tokens); i++ {
					flAddAtTail(&list, tokens[i])
				}
				forwardLists[name] = &list
			}

		case "DOUBLELIST":
			if len(tokens) > 2 {
				list := createDoubleFL(tokens[2])
				for i := 3; i < len(tokens); i++ {
					dblAddAtTail(&list, tokens[i])
				}
				doubleLists[name] = &list
			}

		case "STACK":
			stack := createStack()
			// Загружаем элементы в правильном порядке (первый токен - дно стека)
			for i := 2; i < len(tokens); i++ {
				push(stack, tokens[i])
			}
			stacks[name] = stack

		case "QUEUE":
			queue := createQueue()
			for i := 2; i < len(tokens); i++ {
				enqueue(queue, tokens[i])
			}
			queues[name] = queue
		}
	}
}

func processArrayCommand(args []string) {
	if len(args) < 2 {
		fmt.Println("Ошибка: недостаточно аргументов для команды", args[0])
		return
	}

	name := args[1]

	switch args[0] {
	case "MCREATE":
		if len(args) > 3 {
			fmt.Println("Ошибка: слишком много аргументов для MCREATE")
			return
		}
		if _, exists := arrays[name]; exists {
			fmt.Printf("Ошибка: массив '%s' уже существует\n", name)
			return
		}
		capacity := 10
		if len(args) > 2 {
			cap, err := strconv.Atoi(args[2])
			if err != nil {
				fmt.Println("Ошибка: неверная емкость массива")
				return
			}
			capacity = cap
		}
		arr := createArr(capacity)
		arrays[name] = &arr
		fmt.Printf("Создание массива '%s' успешно\n", name)

	case "MPUSH":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для MPUSH")
			return
		}
		arr, exists := arrays[name]
		if !exists {
			fmt.Printf("Ошибка: массив '%s' не существует\n", name)
			return
		}
		pushBackArr(args[2], arr)
		fmt.Printf("Добавление элемента в массив '%s' успешно\n", name)

	case "MINSERT":
		if len(args) != 4 {
			fmt.Println("Ошибка: неверное количество аргументов для MINSERT")
			return
		}
		arr, exists := arrays[name]
		if !exists {
			fmt.Printf("Ошибка: массив '%s' не существует\n", name)
			return
		}
		index, err := strconv.Atoi(args[2])
		if err != nil || index < 0 || index > arr.size {
			fmt.Println("Ошибка: индекс вне диапазона")
			return
		}
		addAtIndex(index, args[3], arr)
		fmt.Printf("Вставка элемента в массив '%s' успешна\n", name)

	case "MDEL":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для MDEL")
			return
		}
		arr, exists := arrays[name]
		if !exists {
			fmt.Printf("Ошибка: массив '%s' не существует\n", name)
			return
		}
		index, err := strconv.Atoi(args[2])
		if err != nil || index < 0 || index >= arr.size {
			fmt.Println("Ошибка: индекс вне диапазона")
			return
		}
		value, err := getElementIndex(index, *arr)
		if err != nil {
			fmt.Println("Ошибка при получении элемента:", err)
			return
		}
		removeFromIndex(index, arr)
		fmt.Printf("Удаление элемента '%s' из массива '%s' успешно\n", value, name)

	case "MGET":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для MGET")
			return
		}
		arr, exists := arrays[name]
		if !exists {
			fmt.Printf("Ошибка: массив '%s' не существует\n", name)
			return
		}
		index, err := strconv.Atoi(args[2])
		if err != nil || index < 0 || index >= arr.size {
			fmt.Println("Ошибка: индекс вне диапазона")
			return
		}
		value, err := getElementIndex(index, *arr)
		if err != nil {
			fmt.Println("Ошибка при получении элемента:", err)
			return
		}
		fmt.Printf("Элемент массива '%s' по индексу %d: %s\n", name, index, value)

	case "MSET":
		if len(args) != 4 {
			fmt.Println("Ошибка: неверное количество аргументов для MSET")
			return
		}
		arr, exists := arrays[name]
		if !exists {
			fmt.Printf("Ошибка: массив '%s' не существует\n", name)
			return
		}
		index, err := strconv.Atoi(args[2])
		if err != nil || index < 0 || index >= arr.size {
			fmt.Println("Ошибка: индекс вне диапазона")
			return
		}
		replacementElement(index, args[3], arr)
		fmt.Printf("Замена элемента в массиве '%s' успешна\n", name)

	case "MSIZE", "MLENGTH":
		if len(args) != 2 {
			fmt.Println("Ошибка: неверное количество аргументов для", args[0])
			return
		}
		arr, exists := arrays[name]
		if !exists {
			fmt.Printf("Ошибка: массив '%s' не существует\n", name)
			return
		}
		size := genLength(*arr)
		fmt.Printf("Размер массива '%s': %d\n", name, size)
	}
}

func processStackCommand(args []string) {
	if len(args) < 2 {
		fmt.Println("Ошибка: недостаточно аргументов для команды", args[0])
		return
	}

	name := args[1]

	switch args[0] {
	case "SCREATE":
		if len(args) != 2 {
			fmt.Println("Ошибка: неверное количество аргументов для SCREATE")
			return
		}
		if _, exists := stacks[name]; exists {
			fmt.Printf("Ошибка: стек '%s' уже существует\n", name)
			return
		}
		stacks[name] = createStack()
		fmt.Printf("Создание стека '%s' успешно\n", name)

	case "SPUSH":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для SPUSH")
			return
		}
		stack, exists := stacks[name]
		if !exists {
			fmt.Printf("Ошибка: стек '%s' не существует\n", name)
			return
		}
		push(stack, args[2])
		fmt.Printf("Добавление элемента в стек '%s' успешно\n", name)

	case "SPOP":
		if len(args) != 2 {
			fmt.Println("Ошибка: неверное количество аргументов для SPOP")
			return
		}
		stack, exists := stacks[name]
		if !exists {
			fmt.Printf("Ошибка: стек '%s' не существует\n", name)
			return
		}
		if stack.head == nil {
			fmt.Println("Ошибка: стек пуст")
			return
		}
		value := stack.head.value
		pop(stack)
		fmt.Printf("Удаление элемента из стека '%s' успешно. Удаленный элемент: %s\n", name, value)
	}
}

func processQueueCommand(args []string) {
	if len(args) < 2 {
		fmt.Println("Ошибка: недостаточно аргументов для команды", args[0])
		return
	}

	name := args[1]

	switch args[0] {
	case "QCREATE":
		if len(args) != 2 {
			fmt.Println("Ошибка: неверное количество аргументов для QCREATE")
			return
		}
		if _, exists := queues[name]; exists {
			fmt.Printf("Ошибка: очередь '%s' уже существует\n", name)
			return
		}
		queues[name] = createQueue()
		fmt.Printf("Создание очереди '%s' успешно\n", name)

	case "QPUSH":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для QPUSH")
			return
		}
		queue, exists := queues[name]
		if !exists {
			fmt.Printf("Ошибка: очередь '%s' не существует\n", name)
			return
		}
		enqueue(queue, args[2])
		fmt.Printf("Добавление элемента в очередь '%s' успешно\n", name)

	case "QPOP":
		if len(args) != 2 {
			fmt.Println("Ошибка: неверное количество аргументов для QPOP")
			return
		}
		queue, exists := queues[name]
		if !exists {
			fmt.Printf("Ошибка: очередь '%s' не существует\n", name)
			return
		}
		if queue.head == nil {
			fmt.Println("Ошибка: очередь пуста")
			return
		}
		value := queue.head.value
		dequeue(queue)
		fmt.Printf("Удаление элемента из очереди '%s' успешно. Удаленный элемент: %s\n", name, value)
	}
}

func processForwardListCommand(args []string) {
	if len(args) < 2 {
		fmt.Println("Ошибка: недостаточно аргументов для команды", args[0])
		return
	}

	name := args[1]

	switch args[0] {
	case "FCREATE":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для FCREATE")
			return
		}
		if _, exists := forwardLists[name]; exists {
			fmt.Printf("Ошибка: список '%s' уже существует\n", name)
			return
		}
		list := createFL(args[2])
		forwardLists[name] = &list
		fmt.Printf("Создание односвязного списка '%s' успешно\n", name)

	case "FPUSHHEAD":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для FPUSHHEAD")
			return
		}
		list, exists := forwardLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		flAddAtHead(list, args[2])
		fmt.Printf("Добавление элемента в голову списка '%s' успешно\n", name)

	case "FPUSHTAIL":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для FPUSHTAIL")
			return
		}
		list, exists := forwardLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		flAddAtTail(list, args[2])
		fmt.Printf("Добавление элемента в хвост списка '%s' успешно\n", name)

	case "FINSERTBEFORE":
		if len(args) != 4 {
			fmt.Println("Ошибка: неверное количество аргументов для FINSERTBEFORE")
			return
		}
		list, exists := forwardLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		index, err := strconv.Atoi(args[2])
		if err != nil || index < 0 {
			fmt.Println("Ошибка: индекс не может быть отрицательным")
			return
		}
		if list.head == nil {
			fmt.Println("Ошибка: список пуст")
			return
		}
		flAddBeforeIndex(list, args[3], index)
		fmt.Printf("Вставка элемента перед индексом %d списка '%s' успешна\n", index, name)

	case "FINSERTAFTER":
		if len(args) != 4 {
			fmt.Println("Ошибка: неверное количество аргументов для FINSERTAFTER")
			return
		}
		list, exists := forwardLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		index, err := strconv.Atoi(args[2])
		if err != nil || index < 0 {
			fmt.Println("Ошибка: индекс не может быть отрицательным")
			return
		}
		if list.head == nil {
			fmt.Println("Ошибка: список пуст")
			return
		}
		flAddAfterIndex(list, args[3], index)
		fmt.Printf("Вставка элемента после индекса %d списка '%s' успешна\n", index, name)

	case "FDELHEAD":
		if len(args) != 2 {
			fmt.Println("Ошибка: неверное количество аргументов для FDELHEAD")
			return
		}
		list, exists := forwardLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		if list.head == nil {
			fmt.Println("Ошибка: список пуст")
			return
		}
		value := list.head.value
		flDeleteAtHead(list)
		fmt.Printf("Удаление головы списка '%s' успешно. Удаленный элемент: %s\n", name, value)

	case "FDELTAIL":
		if len(args) != 2 {
			fmt.Println("Ошибка: неверное количество аргументов для FDELTAIL")
			return
		}
		list, exists := forwardLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		if list.head == nil {
			fmt.Println("Ошибка: список пуст")
			return
		}
		value := list.tail.value
		flDeleteAtTail(list)
		fmt.Printf("Удаление хвоста списка '%s' успешно. Удаленный элемент: %s\n", name, value)

	case "FDELBEFORE":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для FDELBEFORE")
			return
		}
		list, exists := forwardLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		index, err := strconv.Atoi(args[2])
		if err != nil || index < 0 {
			fmt.Println("Ошибка: индекс не может быть отрицательным")
			return
		}
		if list.head == nil {
			fmt.Println("Ошибка: список пуст")
			return
		}
		if index == 0 {
			fmt.Println("Ошибка: невозможно удалить элемент перед первым элементом")
			return
		}
		flDeleteBeforeIndex(list, index)
		fmt.Printf("Удаление элемента перед индексом %d списка '%s' успешно\n", index, name)

	case "FDELAFTER":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для FDELAFTER")
			return
		}
		list, exists := forwardLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		index, err := strconv.Atoi(args[2])
		if err != nil || index < 0 {
			fmt.Println("Ошибка: индекс не может быть отрицательным")
			return
		}
		if list.head == nil {
			fmt.Println("Ошибка: список пуст")
			return
		}
		flDeleteAfterIndex(list, index)
		fmt.Printf("Удаление элемента после индекса %d списка '%s' успешно\n", index, name)

	case "FDELVALUE":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для FDELVALUE")
			return
		}
		list, exists := forwardLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		if list.head == nil {
			fmt.Println("Ошибка: список пуст")
			return
		}
		flValueDelete(list, args[2])
		fmt.Printf("Удаление элемента '%s' из списка '%s' успешно\n", args[2], name)

	case "FSEARCH":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для FSEARCH")
			return
		}
		list, exists := forwardLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		flSearchNode(*list, args[2])
	}
}

func processDoubleListCommand(args []string) {
	if len(args) < 2 {
		fmt.Println("Ошибка: недостаточно аргументов для команды", args[0])
		return
	}

	name := args[1]

	switch args[0] {
	case "LCREATE":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для LCREATE")
			return
		}
		if _, exists := doubleLists[name]; exists {
			fmt.Printf("Ошибка: список '%s' уже существует\n", name)
			return
		}
		list := createDoubleFL(args[2])
		doubleLists[name] = &list
		fmt.Printf("Создание двусвязного списка '%s' успешно\n", name)

	case "LPUSHHEAD":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для LPUSHHEAD")
			return
		}
		list, exists := doubleLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		dblAddAtHead(list, args[2])
		fmt.Printf("Добавление элемента в голову списка '%s' успешно\n", name)

	case "LPUSHTAIL":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для LPUSHTAIL")
			return
		}
		list, exists := doubleLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		dblAddAtTail(list, args[2])
		fmt.Printf("Добавление элемента в хвост списка '%s' успешно\n", name)

	case "LINSERTBEFORE":
		if len(args) != 4 {
			fmt.Println("Ошибка: неверное количество аргументов для LINSERTBEFORE")
			return
		}
		list, exists := doubleLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		index, err := strconv.Atoi(args[2])
		if err != nil || index < 0 {
			fmt.Println("Ошибка: индекс не может быть отрицательным")
			return
		}
		if list.head == nil {
			fmt.Println("Ошибка: список пуст")
			return
		}
		dblAddBeforeIndex(list, args[3], index)
		fmt.Printf("Вставка элемента перед индексом %d списка '%s' успешна\n", index, name)

	case "LINSERTAFTER":
		if len(args) != 4 {
			fmt.Println("Ошибка: неверное количество аргументов для LINSERTAFTER")
			return
		}
		list, exists := doubleLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		index, err := strconv.Atoi(args[2])
		if err != nil || index < 0 {
			fmt.Println("Ошибка: индекс не может быть отрицательным")
			return
		}
		if list.head == nil {
			fmt.Println("Ошибка: список пуст")
			return
		}
		dblAddAfterIndex(list, args[3], index)
		fmt.Printf("Вставка элемента после индекса %d списка '%s' успешна\n", index, name)

	case "LDELHEAD":
		if len(args) != 2 {
			fmt.Println("Ошибка: неверное количество аргументов для LDELHEAD")
			return
		}
		list, exists := doubleLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		if list.head == nil {
			fmt.Println("Ошибка: список пуст")
			return
		}
		value := list.head.value
		dblDeleteAtHead(list)
		fmt.Printf("Удаление головы списка '%s' успешно. Удаленный элемент: %s\n", name, value)

	case "LDELTAIL":
		if len(args) != 2 {
			fmt.Println("Ошибка: неверное количество аргументов для LDELTAIL")
			return
		}
		list, exists := doubleLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		if list.head == nil {
			fmt.Println("Ошибка: список пуст")
			return
		}
		value := list.tail.value
		dblDeleteAtTail(list)
		fmt.Printf("Удаление хвоста списка '%s' успешно. Удаленный элемент: %s\n", name, value)

	case "LDELBEFORE":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для LDELBEFORE")
			return
		}
		list, exists := doubleLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		index, err := strconv.Atoi(args[2])
		if err != nil || index < 0 {
			fmt.Println("Ошибка: индекс не может быть отрицательным")
			return
		}
		if list.head == nil {
			fmt.Println("Ошибка: список пуст")
			return
		}
		if index == 0 {
			fmt.Println("Ошибка: невозможно удалить элемент перед первым элементом")
			return
		}
		dblDeleteBeforeIndex(list, index)
		fmt.Printf("Удаление элемента перед индексом %d списка '%s' успешно\n", index, name)

	case "LDELAFTER":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для LDELAFTER")
			return
		}
		list, exists := doubleLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		index, err := strconv.Atoi(args[2])
		if err != nil || index < 0 {
			fmt.Println("Ошибка: индекс не может быть отрицательным")
			return
		}
		if list.head == nil {
			fmt.Println("Ошибка: список пуст")
			return
		}
		dblDeleteAfterIndex(list, index)
		fmt.Printf("Удаление элемента после индекса %d списка '%s' успешно\n", index, name)

	case "LDELVALUE":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для LDELVALUE")
			return
		}
		list, exists := doubleLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		if list.head == nil {
			fmt.Println("Ошибка: список пуст")
			return
		}
		dblDeleteValue(list, args[2])
		fmt.Printf("Удаление элемента '%s' из списка '%s' успешно\n", args[2], name)

	case "LSEARCH":
		if len(args) != 3 {
			fmt.Println("Ошибка: неверное количество аргументов для LSEARCH")
			return
		}
		list, exists := doubleLists[name]
		if !exists {
			fmt.Printf("Ошибка: список '%s' не существует\n", name)
			return
		}
		dblSearchValue(list, args[2])
	}
}

func processPrintCommand(args []string) {
	if len(args) != 2 {
		fmt.Println("Ошибка: неверное количество аргументов для PRINT. Ожидается: PRINT <name>")
		return
	}

	name := args[1]

	if arr, exists := arrays[name]; exists {
		fmt.Printf("Содержимое массива '%s': ", name)
		printArray(*arr)
	} else if list, exists := forwardLists[name]; exists {
		fmt.Printf("Содержимое односвязного списка '%s': ", name)
		printFL(list.head)
	} else if list, exists := doubleLists[name]; exists {
		fmt.Printf("Содержимое двусвязного списка '%s': ", name)
		printDoubleFL(list.head)
	} else if stack, exists := stacks[name]; exists {
		fmt.Printf("Содержимое стека '%s': ", name)
		printStack(stack)
	} else if queue, exists := queues[name]; exists {
		fmt.Printf("Содержимое очереди '%s': ", name)
		printQueue(queue)
	} else {
		fmt.Printf("Ошибка: структура данных с именем '%s' не найдена\n", name)
	}
}

func printHelp() {
	fmt.Println("=== СПРАВКА ПО КОМАНДАМ ===")
	fmt.Println("\nМАССИВЫ:")
	fmt.Println("  MCREATE <name> [capacity] - Создать массив")
	fmt.Println("  MPUSH <name> <value>      - Добавить элемент")
	fmt.Println("  MINSERT <name> <index> <value> - Вставить элемент")
	fmt.Println("  MDEL <name> <index>       - Удалить элемент")
	fmt.Println("  MGET <name> <index>       - Получить элемент")
	fmt.Println("  MSET <name> <index> <value> - Заменить элемент")
	fmt.Println("  MSIZE <name>              - Размер массива")
	
	fmt.Println("\nОДНОСВЯЗНЫЕ СПИСКИ:")
	fmt.Println("  FCREATE <name> <value>    - Создать список")
	fmt.Println("  FPUSHHEAD <name> <value>  - Добавить в голову")
	fmt.Println("  FPUSHTAIL <name> <value>  - Добавить в хвост")
	fmt.Println("  FINSERTBEFORE <name> <index> <value> - Добавить перед индексом")
	fmt.Println("  FINSERTAFTER <name> <index> <value> - Добавить после индекса")
	fmt.Println("  FDELHEAD <name>           - Удалить голову")
	fmt.Println("  FDELTAIL <name>           - Удалить хвост")
	fmt.Println("  FDELBEFORE <name> <index> - Удалить перед индексом")
	fmt.Println("  FDELAFTER <name> <index>  - Удалить после индекса")
	fmt.Println("  FDELVALUE <name> <value>  - Удалить по значению")
	fmt.Println("  FSEARCH <name> <value>    - Найти элемент")
	
	fmt.Println("\nДВУСВЯЗНЫЕ СПИСКИ:")
	fmt.Println("  LCREATE <name> <value>    - Создать список")
	fmt.Println("  LPUSHHEAD <name> <value>  - Добавить в голову")
	fmt.Println("  LPUSHTAIL <name> <value>  - Добавить в хвост")
	fmt.Println("  LINSERTBEFORE <name> <index> <value> - Добавить перед индексом")
	fmt.Println("  LINSERTAFTER <name> <index> <value> - Добавить после индекса")
	fmt.Println("  LDELHEAD <name>           - Удалить голову")
	fmt.Println("  LDELTAIL <name>           - Удалить хвост")
	fmt.Println("  LDELBEFORE <name> <index> - Удалить перед индексом")
	fmt.Println("  LDELAFTER <name> <index>  - Удалить после индекса")
	fmt.Println("  LDELVALUE <name> <value>  - Удалить по значению")
	fmt.Println("  LSEARCH <name> <value>    - Найти элемент")
	
	fmt.Println("\nСТЕКИ:")
	fmt.Println("  SCREATE <name>            - Создать стек")
	fmt.Println("  SPUSH <name> <value>      - Добавить элемент")
	fmt.Println("  SPOP <name>               - Удалить элемент")
	
	fmt.Println("\nОЧЕРЕДИ:")
	fmt.Println("  QCREATE <name>            - Создать очередь")
	fmt.Println("  QPUSH <name> <value>      - Добавить элемент")
	fmt.Println("  QPOP <name>               - Удалить элемент")
	
	fmt.Println("\nОБЩИЕ КОМАНДЫ:")
	fmt.Println("  PRINT <name>              - Показать содержимое структуры")
	fmt.Println("  HELP                      - Справка")
	
	fmt.Println("\nПРИМЕРЫ ЗАПУСКА:")
	fmt.Println("  ./dbms --file data.txt --query 'MCREATE myarr'")
	fmt.Println("  ./dbms --file data.txt --query 'MPUSH myarr value'")
	fmt.Println("  ./dbms --file data.txt --query 'PRINT myarr'")
	fmt.Println("  ./dbms --help")
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Ошибка: неверное количество аргументов. Используйте --help для справки.")
		os.Exit(1)
	}
	
	var filename, query string
	
	for i := 1; i < len(os.Args); i++ {
		arg := os.Args[i]
		if arg == "--file" && i+1 < len(os.Args) {
			filename = os.Args[i+1]
			i++
		} else if arg == "--query" && i+1 < len(os.Args) {
			query = os.Args[i+1]
			i++
		} else if arg == "--help" || arg == "-h" {
			printHelp()
			return
		} else {
			fmt.Printf("Ошибка: неизвестный аргумент %s\n", arg)
			os.Exit(1)
		}
	}
	
	if filename == "" || query == "" {
		fmt.Println("Ошибка: необходимо указать --file и --query")
		os.Exit(1)
	}
	
	loadFromFile(filename)
	
	args := strings.Fields(query)
	if len(args) == 0 {
		fmt.Println("Ошибка: пустой запрос")
		os.Exit(1)
	}
	
	command := args[0]
	
	switch command {
	case "MCREATE", "MPUSH", "MINSERT", "MDEL", "MGET", "MSET", "MSIZE", "MLENGTH":
		processArrayCommand(args)
	case "FCREATE", "FPUSHHEAD", "FPUSHTAIL", "FINSERTBEFORE", "FINSERTAFTER", 
	     "FDELHEAD", "FDELTAIL", "FDELBEFORE", "FDELAFTER", "FDELVALUE", "FSEARCH":
		processForwardListCommand(args)
	case "LCREATE", "LPUSHHEAD", "LPUSHTAIL", "LINSERTBEFORE", "LINSERTAFTER", 
	     "LDELHEAD", "LDELTAIL", "LDELBEFORE", "LDELAFTER", "LDELVALUE", "LSEARCH":
		processDoubleListCommand(args)
	case "SCREATE", "SPUSH", "SPOP":
		processStackCommand(args)
	case "QCREATE", "QPUSH", "QPOP":
		processQueueCommand(args)
	case "PRINT":
		processPrintCommand(args)
	case "HELP":
		printHelp()
	default:
		fmt.Printf("Ошибка: неизвестная команда '%s'\n", command)
		fmt.Println("Используйте HELP для просмотра доступных команд.")
		os.Exit(1)
	}
	
	saveToFile(filename)
}
