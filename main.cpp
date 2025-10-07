#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <fstream>
#include "stack.h"
#include "queue.h"
#include "array.h"
#include "forwardlist.h"
#include "doubleFL.h"
#include "tree.h"

using namespace std;

map<string, DynArray> arrays;
map<string, forwardList> forwardLists;
map<string, doubleFL> doubleLists;
map<string, Stack*> stacks;
map<string, Queue*> queues;
map<string, Tree*> trees;

void saveToFile(const string& filename);
void loadFromFile(const string& filename);
void processArrayCommand(const vector<string>& args);
void processForwardListCommand(const vector<string>& args);
void processDoubleListCommand(const vector<string>& args);
void processStackCommand(const vector<string>& args);
void processQueueCommand(const vector<string>& args);
void processTreeCommand(const vector<string>& args);
void processPrintCommand(const vector<string>& args);
vector<string> split(const string& str);
void printHelp();

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Ошибка: неверное количество аргументов. Используйте --help для справки." << endl;
        return 1;
    }
    
    string filename;
    string query;
    
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[++i];
        }
        else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
        else if (arg == "--help" || arg == "-h") {
            printHelp();
            return 0;
        }
        else {
            cerr << "Ошибка: неизвестный аргумент " << arg << endl;
            return 1;
        }
    }
    
    if (filename.empty() || query.empty()) {
        cerr << "Ошибка: необходимо указать --file и --query" << endl;
        return 1;
    }
    
    loadFromFile(filename);
    
    vector<string> args = split(query);
    if (args.empty()) {
        cerr << "Ошибка: пустой запрос" << endl;
        return 1;
    }
    
    string command = args[0];
    
    if (command == "MCREATE" || command == "MPUSH" || command == "MINSERT" || 
        command == "MDEL" || command == "MGET" || command == "MSET" || 
        command == "MSIZE" || command == "MLENGTH") {
        processArrayCommand(args);
    }
    else if (command == "FCREATE" || command == "FPUSHHEAD" || command == "FPUSHTAIL" || 
             command == "FINSERTBEFORE" || command == "FINSERTAFTER" || 
             command == "FDELHEAD" || command == "FDELTAIL" ||
             command == "FDELBEFORE" || command == "FDELAFTER" ||
             command == "FDELVALUE" || command == "FSEARCH") {
        processForwardListCommand(args);
    }
    else if (command == "LCREATE" || command == "LPUSHHEAD" || command == "LPUSHTAIL" || 
             command == "LINSERTBEFORE" || command == "LINSERTAFTER" || 
             command == "LDELHEAD" || command == "LDELTAIL" ||
             command == "LDELBEFORE" || command == "LDELAFTER" ||
             command == "LDELVALUE" || command == "LSEARCH") {
        processDoubleListCommand(args);
    }
    else if (command == "SCREATE" || command == "SPUSH" || command == "SPOP") {
        processStackCommand(args);
    }
    else if (command == "QCREATE" || command == "QPUSH" || command == "QPOP") {
        processQueueCommand(args);
    }
    else if (command == "TCREATE" || command == "TINSERT" || command == "TDELETE" || 
             command == "TSEARCH") {
        processTreeCommand(args);
    }
    else if (command == "PRINT") {
        processPrintCommand(args);
    }
    else if (command == "HELP") {
        printHelp();
    }
    else {
        cerr << "Неизвестная команда: " << command << endl;
        return 1;
    }
    
    saveToFile(filename);
    
    for (auto& pair : stacks) {
        while (pair.second->head != nullptr) {
            SNode* temp = pair.second->head;
            pair.second->head = pair.second->head->next;
            delete temp;
        }
        delete pair.second;
    }
    
    for (auto& pair : queues) {
        while (pair.second->head != nullptr) {
            QNode* temp = pair.second->head;
            pair.second->head = pair.second->head->next;
            delete temp;
        }
        delete pair.second;
    }
    
    for (auto& pair : trees) {
        removeTree(pair.second);
    }
    
    return 0;
}

void processTreeCommand(const vector<string>& args) {
    if (args.size() < 2) {
        cerr << "Ошибка: недостаточно аргументов для команды " << args[0] << endl;
        return;
    }
    
    string name = args[1];
    
    try {
        if (args[0] == "TCREATE") {
            if (args.size() != 2) {
                cerr << "Ошибка: неверное количество аргументов для TCREATE" << endl;
                return;
            }
            if (trees.find(name) != trees.end()) {
                cerr << "Ошибка: дерево '" << name << "' уже существует" << endl;
                return;
            }
            trees[name] = createTree();
            cout << "Создание красно-черного дерева '" << name << "' успешно" << endl;
        }
        else if (args[0] == "TINSERT") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для TINSERT" << endl;
                return;
            }
            if (trees.find(name) == trees.end()) {
                cerr << "Ошибка: дерево '" << name << "' не существует" << endl;
                return;
            }
            int value = stoi(args[2]);
            insert(trees[name], value);
            cout << "Вставка элемента " << value << " в дерево '" << name << "' успешна" << endl;
        }
        else if (args[0] == "TDELETE") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для TDELETE" << endl;
                return;
            }
            if (trees.find(name) == trees.end()) {
                cerr << "Ошибка: дерево '" << name << "' не существует" << endl;
                return;
            }
            int value = stoi(args[2]);
            try {
                deleteNode(trees[name], value);
                cout << "Удаление элемента " << value << " из дерева '" << name << "' успешно" << endl;
            } catch (const runtime_error& e) {
                cerr << "Ошибка при удалении: " << e.what() << endl;
            }
        }
        else if (args[0] == "TSEARCH") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для TSEARCH" << endl;
                return;
            }
            if (trees.find(name) == trees.end()) {
                cerr << "Ошибка: дерево '" << name << "' не существует" << endl;
                return;
            }
            int value = stoi(args[2]);
            TNode* found = findNode(trees[name], value);
            if (found != trees[name]->nil) {
                cout << "Элемент " << value << " найден в дереве '" << name << "'" << endl;
            } else {
                cout << "Элемент " << value << " не найден в дереве '" << name << "'" << endl;
            }
        }
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

void processPrintCommand(const vector<string>& args) {
    if (args.size() != 2) {
        cerr << "Ошибка: неверное количество аргументов для PRINT. Ожидается: PRINT <name>" << endl;
        return;
    }
    
    string name = args[1];
    
    if (arrays.find(name) != arrays.end()) {
        cout << "Содержимое массива '" << name << "': ";
        printArray(arrays[name]);
    }
    else if (forwardLists.find(name) != forwardLists.end()) {
        cout << "Содержимое односвязного списка '" << name << "': ";
        printFL(forwardLists[name].head);
    }
    else if (doubleLists.find(name) != doubleLists.end()) {
        cout << "Содержимое двусвязного списка '" << name << "': ";
        printDoubleFL(doubleLists[name].head);
    }
    else if (stacks.find(name) != stacks.end()) {
        cout << "Содержимое стека '" << name << "': ";
        printStack(stacks[name]);
    }
    else if (queues.find(name) != queues.end()) {
        cout << "Содержимое очереди '" << name << "': ";
        printQueue(queues[name]);
    }
    else if (trees.find(name) != trees.end()) {
        cout << "Содержимое красно-черного дерева '" << name << "':" << endl;
        printTree(trees[name]);
    }
    else {
        cerr << "Ошибка: структура данных с именем '" << name << "' не найдена" << endl;
    }
}

void processArrayCommand(const vector<string>& args) {
    string name = args[1];
    
    try {
        if (args[0] == "MCREATE") {
            if (args.size() > 3) {
                cerr << "Ошибка: слишком много аргументов для MCREATE" << endl;
                return;
            }
            if (arrays.find(name) != arrays.end()) {
                cerr << "Ошибка: массив '" << name << "' уже существует" << endl;
                return;
            }
            int capacity = args.size() > 2 ? stoi(args[2]) : 10;
            createArr(arrays[name], capacity);
            cout << "Создание массива '" << name << "' успешно" << endl;
        }
        else if (args[0] == "MPUSH") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для MPUSH" << endl;
                return;
            }
            if (arrays.find(name) == arrays.end()) {
                cerr << "Ошибка: массив '" << name << "' не существует" << endl;
                return;
            }
            pushBackArr(args[2], arrays[name]);
            cout << "Добавление элемента в массив '" << name << "' успешно" << endl;
        }
        else if (args[0] == "MINSERT") {
            if (args.size() != 4) {
                cerr << "Ошибка: неверное количество аргументов для MINSERT" << endl;
                return;
            }
            if (arrays.find(name) == arrays.end()) {
                cerr << "Ошибка: массив '" << name << "' не существует" << endl;
                return;
            }
            int index = stoi(args[2]);
            if (index < 0 || index > arrays[name].size) {
                cerr << "Ошибка: индекс вне диапазона" << endl;
                return;
            }
            addAtIndex(index, args[3], arrays[name]);
            cout << "Вставка элемента в массив '" << name << "' успешна" << endl;
        }
        else if (args[0] == "MDEL") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для MDEL" << endl;
                return;
            }
            if (arrays.find(name) == arrays.end()) {
                cerr << "Ошибка: массив '" << name << "' не существует" << endl;
                return;
            }
            int index = stoi(args[2]);
            if (index < 0 || index >= arrays[name].size) {
                cerr << "Ошибка: индекс вне диапазона" << endl;
                return;
            }
            string value = getElementIndex(index, arrays[name]);
            removeFromIndex(index, arrays[name]);
            cout << "Удаление элемента '" << value << "' из массива '" << name << "' успешно" << endl;
        }
        else if (args[0] == "MGET") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для MGET" << endl;
                return;
            }
            if (arrays.find(name) == arrays.end()) {
                cerr << "Ошибка: массив '" << name << "' не существует" << endl;
                return;
            }
            int index = stoi(args[2]);
            if (index < 0 || index >= arrays[name].size) {
                cerr << "Ошибка: индекс вне диапазона" << endl;
                return;
            }
            string value = getElementIndex(index, arrays[name]);
            cout << "Элемент массива '" << name << "' по индексу " << index << ": " << value << endl;
        }
        else if (args[0] == "MSET") {
            if (args.size() != 4) {
                cerr << "Ошибка: неверное количество аргументов для MSET" << endl;
                return;
            }
            if (arrays.find(name) == arrays.end()) {
                cerr << "Ошибка: массив '" << name << "' не существует" << endl;
                return;
            }
            int index = stoi(args[2]);
            if (index < 0 || index >= arrays[name].size) {
                cerr << "Ошибка: индекс вне диапазона" << endl;
                return;
            }
            replacementElement(index, args[3], arrays[name]);
            cout << "Замена элемента в массиве '" << name << "' успешна" << endl;
        }
        else if (args[0] == "MSIZE") {
            if (args.size() != 2) {
                cerr << "Ошибка: неверное количество аргументов для MSIZE" << endl;
                return;
            }
            if (arrays.find(name) == arrays.end()) {
                cerr << "Ошибка: массив '" << name << "' не существует" << endl;
                return;
            }
            int size = genLength(arrays[name]);
            cout << "Размер массива '" << name << "': " << size << endl;
        }
        else if (args[0] == "MLENGTH") {
            if (args.size() != 2) {
                cerr << "Ошибка: неверное количество аргументов для MLENGTH. Ожидается: MLENGTH <name>" << endl;
                return;
            }
            if (arrays.find(name) == arrays.end()) {
                cerr << "Ошибка: массив '" << name << "' не существует" << endl;
                return;
            }
            int length = genLength(arrays[name]);
            cout << "Длина массива '" << name << "': " << length << endl;
        }
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

void processStackCommand(const vector<string>& args) {
    string name = args[1];
    
    try {
        if (args[0] == "SCREATE") {
            if (args.size() != 2) {
                cerr << "Ошибка: неверное количество аргументов для SCREATE" << endl;
                return;
            }
            if (stacks.find(name) != stacks.end()) {
                cerr << "Ошибка: стек '" << name << "' уже существует" << endl;
                return;
            }
            stacks[name] = new Stack;
            createStack(stacks[name]);
            cout << "Создание стека '" << name << "' успешно" << endl;
        }
        else if (args[0] == "SPUSH") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для SPUSH" << endl;
                return;
            }
            if (stacks.find(name) == stacks.end()) {
                cerr << "Ошибка: стек '" << name << "' не существует" << endl;
                return;
            }
            push(stacks[name], args[2]);
            cout << "Добавление элемента в стек '" << name << "' успешно" << endl;
        }
        else if (args[0] == "SPOP") {
            if (args.size() != 2) {
                cerr << "Ошибка: неверное количество аргументов для SPOP" << endl;
                return;
            }
            if (stacks.find(name) == stacks.end()) {
                cerr << "Ошибка: стек '" << name << "' не существует" << endl;
                return;
            }
            if (stacks[name]->head == nullptr) {
                cerr << "Ошибка: стек пуст" << endl;
                return;
            }
            string value = stacks[name]->head->value;
            pop(stacks[name]);
            cout << "Удаление элемента из стека '" << name << "' успешно. Удаленный элемент: " << value << endl;
        }
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

void processQueueCommand(const vector<string>& args) {
    string name = args[1];
    
    try {
        if (args[0] == "QCREATE") {
            if (args.size() != 2) {
                cerr << "Ошибка: неверное количество аргументов для QCREATE" << endl;
                return;
            }
            if (queues.find(name) != queues.end()) {
                cerr << "Ошибка: очередь '" << name << "' уже существует" << endl;
                return;
            }
            queues[name] = new Queue;
            createQueue(queues[name]);
            cout << "Создание очереди '" << name << "' успешно" << endl;
        }
        else if (args[0] == "QPUSH") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для QPUSH" << endl;
                return;
            }
            if (queues.find(name) == queues.end()) {
                cerr << "Ошибка: очередь '" << name << "' не существует" << endl;
                return;
            }
            enqueue(queues[name], args[2]);
            cout << "Добавление элемента в очередь '" << name << "' успешно" << endl;
        }
        else if (args[0] == "QPOP") {
            if (args.size() != 2) {
                cerr << "Ошибка: неверное количество аргументов для QPOP" << endl;
                return;
            }
            if (queues.find(name) == queues.end()) {
                cerr << "Ошибка: очередь '" << name << "' не существует" << endl;
                return;
            }
            if (queues[name]->head == nullptr) {
                cerr << "Ошибка: очередь пуста" << endl;
                return;
            }
            string value = queues[name]->head->value;
            dequeue(queues[name]);
            cout << "Удаление элемента из очереди '" << name << "' успешно. Удаленный элемент: " << value << endl;
        }
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

void processForwardListCommand(const vector<string>& args) {
    if (args.size() < 2) {
        cerr << "Ошибка: недостаточно аргументов для команды " << args[0] << endl;
        return;
    }
    
    string name = args[1];
    
    try {
        if (args[0] == "FCREATE") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для FCREATE" << endl;
                return;
            }
            if (forwardLists.find(name) != forwardLists.end()) {
                cerr << "Ошибка: список '" << name << "' уже существует" << endl;
                return;
            }
            createFL(forwardLists[name], args[2]);
            cout << "Создание односвязного списка '" << name << "' успешно" << endl;
        }
        else if (args[0] == "FPUSHHEAD") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для FPUSHHEAD" << endl;
                return;
            }
            if (forwardLists.find(name) == forwardLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            addAtHead(forwardLists[name], args[2]);
            cout << "Добавление элемента в голову списка '" << name << "' успешно" << endl;
        }
        else if (args[0] == "FPUSHTAIL") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для FPUSHTAIL" << endl;
                return;
            }
            if (forwardLists.find(name) == forwardLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            addAtTail(forwardLists[name], args[2]);
            cout << "Добавление элемента в хвост списка '" << name << "' успешно" << endl;
        }
        else if (args[0] == "FINSERTBEFORE") {
            if (args.size() != 4) {
                cerr << "Ошибка: неверное количество аргументов для FINSERTBEFORE" << endl;
                return;
            }
            if (forwardLists.find(name) == forwardLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            int index = stoi(args[2]);
            if (index < 0) {
                cerr << "Ошибка: индекс не может быть отрицательным" << endl;
                return;
            }
            if (forwardLists[name].head == nullptr) {
                cerr << "Ошибка: список пуст" << endl;
                return;
            }
            addBeforeIndex(forwardLists[name], args[3], index);
            cout << "Вставка элемента перед индексом " << index << " списка '" << name << "' успешна" << endl;
        }
        else if (args[0] == "FINSERTAFTER") {
            if (args.size() != 4) {
                cerr << "Ошибка: неверное количество аргументов для FINSERTAFTER" << endl;
                return;
            }
            if (forwardLists.find(name) == forwardLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            int index = stoi(args[2]);
            if (index < 0) {
                cerr << "Ошибка: индекс не может быть отрицательным" << endl;
                return;
            }
            if (forwardLists[name].head == nullptr) {
                cerr << "Ошибка: список пуст" << endl;
                return;
            }
            addAfterIndex(forwardLists[name], args[3], index);
            cout << "Вставка элемента после индекса " << index << " списка '" << name << "' успешна" << endl;
        }
        else if (args[0] == "FDELHEAD") {
            if (args.size() != 2) {
                cerr << "Ошибка: неверное количество аргументов для FDELHEAD" << endl;
                return;
            }
            if (forwardLists.find(name) == forwardLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            if (forwardLists[name].head == nullptr) {
                cerr << "Ошибка: список пуст" << endl;
                return;
            }
            string value = forwardLists[name].head->value;
            deleteAtHead(forwardLists[name]);
            cout << "Удаление головы списка '" << name << "' успешно. Удаленный элемент: " << value << endl;
        }
        else if (args[0] == "FDELTAIL") {
            if (args.size() != 2) {
                cerr << "Ошибка: неверное количество аргументов для FDELTAIL" << endl;
                return;
            }
            if (forwardLists.find(name) == forwardLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            if (forwardLists[name].head == nullptr) {
                cerr << "Ошибка: список пуст" << endl;
                return;
            }
            string value = forwardLists[name].tail->value;
            deleteAtTail(forwardLists[name]);
            cout << "Удаление хвоста списка '" << name << "' успешно. Удаленный элемент: " << value << endl;
        }
        else if (args[0] == "FDELBEFORE") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для FDELBEFORE" << endl;
                return;
            }
            if (forwardLists.find(name) == forwardLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            int index = stoi(args[2]);
            if (index < 0) {
                cerr << "Ошибка: индекс не может быть отрицательным" << endl;
                return;
            }
            if (forwardLists[name].head == nullptr) {
                cerr << "Ошибка: список пуст" << endl;
                return;
            }
            if (index == 0) {
                cerr << "Ошибка: невозможно удалить элемент перед первым элементом" << endl;
                return;
            }
            deleteBeforeIndex(forwardLists[name], index);
            cout << "Удаление элемента перед индексом " << index << " списка '" << name << "'." << endl;
        }
        else if (args[0] == "FDELAFTER") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для FDELAFTER" << endl;
                return;
            }
            if (forwardLists.find(name) == forwardLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            int index = stoi(args[2]);
            if (index < 0) {
                cerr << "Ошибка: индекс не может быть отрицательным" << endl;
                return;
            }
            if (forwardLists[name].head == nullptr) {
                cerr << "Ошибка: список пуст" << endl;
                return;
            }
            deleteAfterIndex(forwardLists[name], index);
            cout << "Удаление элемента после индекса " << index << " списка '" << name << "' успешно" << endl;
        }
        else if (args[0] == "FDELVALUE") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для FDELVALUE" << endl;
                return;
            }
            if (forwardLists.find(name) == forwardLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            if (forwardLists[name].head == nullptr) {
                cerr << "Ошибка: список пуст" << endl;
                return;
            }
            valueDelete(forwardLists[name], args[2]);
            cout << "Удаление элемента '" << args[2] << "' из списка '" << name << "'..." << endl;
        }
        else if (args[0] == "FSEARCH") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для FSEARCH" << endl;
                return;
            }
            if (forwardLists.find(name) == forwardLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            searchNode(forwardLists[name], args[2]);
        }
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

void processDoubleListCommand(const vector<string>& args) {
    if (args.size() < 2) {
        cerr << "Ошибка: недостаточно аргументов для команды " << args[0] << endl;
        return;
    }
    
    string name = args[1];
    
    try {
        if (args[0] == "LCREATE") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для LCREATE" << endl;
                return;
            }
            if (doubleLists.find(name) != doubleLists.end()) {
                cerr << "Ошибка: список '" << name << "' уже существует" << endl;
                return;
            }
            createDoubleFL(args[2], doubleLists[name]);
            cout << "Создание двусвязного списка '" << name << "' успешно" << endl;
        }
        else if (args[0] == "LPUSHHEAD") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для LPUSHHEAD" << endl;
                return;
            }
            if (doubleLists.find(name) == doubleLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            addAtHead(doubleLists[name], args[2]);
            cout << "Добавление элемента в голову списка '" << name << "' успешно" << endl;
        }
        else if (args[0] == "LPUSHTAIL") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для LPUSHTAIL" << endl;
                return;
            }
            if (doubleLists.find(name) == doubleLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            addAtTail(doubleLists[name], args[2]);
            cout << "Добавление элемента в хвост списка '" << name << "' успешно" << endl;
        }
        else if (args[0] == "LINSERTBEFORE") {
            if (args.size() != 4) {
                cerr << "Ошибка: неверное количество аргументов для LINSERTBEFORE" << endl;
                return;
            }
            if (doubleLists.find(name) == doubleLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            int index = stoi(args[2]);
            if (index < 0) {
                cerr << "Ошибка: индекс не может быть отрицательным" << endl;
                return;
            }
            if (doubleLists[name].head == nullptr) {
                cerr << "Ошибка: список пуст" << endl;
                return;
            }
            addBeforeIndex(doubleLists[name], args[3], index);
            cout << "Вставка элемента перед индексом " << index << " списка '" << name << "' успешна" << endl;
        }
        else if (args[0] == "LINSERTAFTER") {
            if (args.size() != 4) {
                cerr << "Ошибка: неверное количество аргументов для LINSERTAFTER" << endl;
                return;
            }
            if (doubleLists.find(name) == doubleLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            int index = stoi(args[2]);
            if (index < 0) {
                cerr << "Ошибка: индекс не может быть отрицательным" << endl;
                return;
            }
            if (doubleLists[name].head == nullptr) {
                cerr << "Ошибка: список пуст" << endl;
                return;
            }
            addAfterIndex(doubleLists[name], args[3], index);
            cout << "Вставка элемента после индекса " << index << " списка '" << name << "' успешна" << endl;
        }
        else if (args[0] == "LDELHEAD") {
            if (args.size() != 2) {
                cerr << "Ошибка: неверное количество аргументов для LDELHEAD" << endl;
                return;
            }
            if (doubleLists.find(name) == doubleLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            if (doubleLists[name].head == nullptr) {
                cerr << "Ошибка: список пуст" << endl;
                return;
            }
            string value = doubleLists[name].head->value;
            deleteAtHead(doubleLists[name]);
            cout << "Удаление головы списка '" << name << "' успешно. Удаленный элемент: " << value << endl;
        }
        else if (args[0] == "LDELTAIL") {
            if (args.size() != 2) {
                cerr << "Ошибка: неверное количество аргументов для LDELTAIL" << endl;
                return;
            }
            if (doubleLists.find(name) == doubleLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            if (doubleLists[name].head == nullptr) {
                cerr << "Ошибка: список пуст" << endl;
                return;
            }
            string value = doubleLists[name].tail->value;
            deleteAtTail(doubleLists[name]);
            cout << "Удаление хвоста списка '" << name << "' успешно. Удаленный элемент: " << value << endl;
        }
        else if (args[0] == "LDELBEFORE") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для LDELBEFORE" << endl;
                return;
            }
            if (doubleLists.find(name) == doubleLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            int index = stoi(args[2]);
            if (index < 0) {
                cerr << "Ошибка: индекс не может быть отрицательным" << endl;
                return;
            }
            if (doubleLists[name].head == nullptr) {
                cerr << "Ошибка: список пуст" << endl;
                return;
            }
            if (index == 0) {
                cerr << "Ошибка: невозможно удалить элемент перед первым элементом" << endl;
                return;
            }
            deleteBeforeIndex(doubleLists[name], index);
            cout << "Удаление элемента перед индексом " << index << " списка '" << name << "' успешно" << endl;
        }
        else if (args[0] == "LDELAFTER") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для LDELAFTER" << endl;
                return;
            }
            if (doubleLists.find(name) == doubleLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            int index = stoi(args[2]);
            if (index < 0) {
                cerr << "Ошибка: индекс не может быть отрицательным" << endl;
                return;
            }
            if (doubleLists[name].head == nullptr) {
                cerr << "Ошибка: список пуст" << endl;
                return;
            }
            deleteAfterIndex(doubleLists[name], index);
            cout << "Удаление элемента после индекса " << index << " списка '" << name << "' успешно" << endl;
        }
        else if (args[0] == "LDELVALUE") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для LDELVALUE" << endl;
                return;
            }
            if (doubleLists.find(name) == doubleLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            if (doubleLists[name].head == nullptr) {
                cerr << "Ошибка: список пуст" << endl;
                return;
            }
            deleteValue(doubleLists[name], args[2]);
            cout << "Удаление элемента '" << args[2] << "' из списка '" << name << "' успешно" << endl;
        }
        else if (args[0] == "LSEARCH") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для LSEARCH" << endl;
                return;
            }
            if (doubleLists.find(name) == doubleLists.end()) {
                cerr << "Ошибка: список '" << name << "' не существует" << endl;
                return;
            }
            searchValue(doubleLists[name], args[2]);
        }
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

void saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл для записи" << endl;
        return;
    }
    
    for (const auto& pair : arrays) {
        file << "ARRAY " << pair.first << " ";
        for (int i = 0; i < pair.second.size; i++) {
            file << pair.second.data[i];
            if (i < pair.second.size - 1) file << " ";
        }
        file << endl;
    }
    
    for (const auto& pair : forwardLists) {
        file << "FORWARDLIST " << pair.first << " ";
        FNode* current = pair.second.head;
        while (current != nullptr) {
            file << current->value;
            if (current->next != nullptr) file << " ";
            current = current->next;
        }
        file << endl;
    }
    
    for (const auto& pair : doubleLists) {
        file << "DOUBLELIST " << pair.first << " ";
        DNode* current = pair.second.head;
        while (current != nullptr) {
            file << current->value;
            if (current->next != nullptr) file << " ";
            current = current->next;
        }
        file << endl;
    }
    
    for (const auto& pair : stacks) {
        file << "STACK " << pair.first << " ";
        SNode* current = pair.second->head;
        while (current != nullptr) {
            file << current->value;
            if (current->next != nullptr) file << " ";
            current = current->next;
        }
        file << endl;
    }
    
    for (const auto& pair : queues) {
        file << "QUEUE " << pair.first << " ";
        QNode* current = pair.second->head;
        while (current != nullptr) {
            file << current->value;
            if (current->next != nullptr) file << " ";
            current = current->next;
        }
        file << endl;
    }
    
    for (const auto& pair : trees) {
        file << "TREE " << pair.first << " ";
        saveTreeToStream(pair.second, file);
        file << endl;
    }
    
    file.close();
}

void loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string type, name;
        ss >> type >> name;
        
        if (type == "ARRAY") {
            string value;
            createArr(arrays[name], 10);
            while (ss >> value) {
                pushBackArr(value, arrays[name]);
            }
        }
        else if (type == "FORWARDLIST") {
            string value;
            bool first = true;
            while (ss >> value) {
                if (first) {
                    createFL(forwardLists[name], value);
                    first = false;
                } else {
                    addAtTail(forwardLists[name], value);
                }
            }
        }
        else if (type == "DOUBLELIST") {
            string value;
            bool first = true;
            while (ss >> value) {
                if (first) {
                    createDoubleFL(value, doubleLists[name]);
                    first = false;
                } else {
                    addAtTail(doubleLists[name], value);
                }
            }
        }
        else if (type == "STACK") {
            vector<string> values;
            string value;
            while (ss >> value) {
                values.push_back(value);
            }
            if (stacks.find(name) == stacks.end()) {
                stacks[name] = new Stack;
            }
            createStack(stacks[name]);
            for (auto it = values.rbegin(); it != values.rend(); ++it) {
                push(stacks[name], *it);
            }
        }
        else if (type == "QUEUE") {
            if (queues.find(name) == queues.end()) {
                queues[name] = new Queue;
            }
            createQueue(queues[name]);
            string value;
            while (ss >> value) {
                enqueue(queues[name], value);
            }
        }
        else if (type == "TREE") {
            if (trees.find(name) == trees.end()) {
                trees[name] = createTree();
            } 
            else {
                removeTree(trees[name]);
                trees[name] = createTree();
            }
            loadTreeFromStream(trees[name], ss);
        }
    }
    
    file.close();
}

vector<string> split(const string& str) {
    vector<string> result;
    stringstream ss(str);
    string item;
    
    while (ss >> item) {
        if (!item.empty() && item.front() == '\'' && item.back() == '\'') {
            item = item.substr(1, item.size() - 2);
        }
        result.push_back(item);
    }
    
    return result;
}

void printHelp() {
    cout << "=== СПРАВКА ПО КОМАНДАМ ===" << endl;
    cout << "\nМАССИВЫ:" << endl;
    cout << "  MCREATE <name> [capacity] - Создать массив" << endl;
    cout << "  MPUSH <name> <value>      - Добавить элемент" << endl;
    cout << "  MINSERT <name> <index> <value> - Вставить элемент" << endl;
    cout << "  MDEL <name> <index>       - Удалить элемент" << endl;
    cout << "  MGET <name> <index>       - Получить элемент" << endl;
    cout << "  MSET <name> <index> <value> - Заменить элемент" << endl;
    cout << "  MSIZE <name>              - Размер массива" << endl;
    
    cout << "\nОДНОСВЯЗНЫЕ СПИСКИ:" << endl;
    cout << "  FCREATE <name> <value>    - Создать список" << endl;
    cout << "  FPUSHHEAD <name> <value>  - Добавить в голову" << endl;
    cout << "  FPUSHTAIL <name> <value>  - Добавить в хвост" << endl;
    cout << "  FINSERTBEFORE <name> <index> <value> - Добавить перед индексом" << endl;
    cout << "  FINSERTAFTER <name> <index> <value> - Добавить после индекса" << endl;
    cout << "  FDELHEAD <name>           - Удалить голову" << endl;
    cout << "  FDELTAIL <name>           - Удалить хвост" << endl;
    cout << "  FDELBEFORE <name> <index> - Удалить перед индексом" << endl;
    cout << "  FDELAFTER <name> <index>  - Удалить после индекса" << endl;
    cout << "  FDELVALUE <name> <value>  - Удалить по значению" << endl;
    cout << "  FSEARCH <name> <value>    - Найти элемент" << endl;
    
    cout << "\nДВУСВЯЗНЫЕ СПИСКИ:" << endl;
    cout << "  LCREATE <name> <value>    - Создать список" << endl;
    cout << "  LPUSHHEAD <name> <value>  - Добавить в голову" << endl;
    cout << "  LPUSHTAIL <name> <value>  - Добавить в хвост" << endl;
    cout << "  LINSERTBEFORE <name> <index> <value> - Добавить перед индексом" << endl;
    cout << "  LINSERTAFTER <name> <index> <value> - Добавить после индекса" << endl;
    cout << "  LDELHEAD <name>           - Удалить голову" << endl;
    cout << "  LDELTAIL <name>           - Удалить хвост" << endl;
    cout << "  LDELBEFORE <name> <index> - Удалить перед индексом" << endl;
    cout << "  LDELAFTER <name> <index>  - Удалить после индекса" << endl;
    cout << "  LDELVALUE <name> <value>  - Удалить по значению" << endl;
    cout << "  LSEARCH <name> <value>    - Найти элемент" << endl;
    
    cout << "\nСТЕКИ:" << endl;
    cout << "  SCREATE <name>            - Создать стек" << endl;
    cout << "  SPUSH <name> <value>      - Добавить элемент" << endl;
    cout << "  SPOP <name>               - Удалить элемент" << endl;
    
    cout << "\nОЧЕРЕДИ:" << endl;
    cout << "  QCREATE <name>            - Создать очередь" << endl;
    cout << "  QPUSH <name> <value>      - Добавить элемент" << endl;
    cout << "  QPOP <name>               - Удалить элемент" << endl;
    
    cout << "\nКРАСНО-ЧЕРНЫЕ ДЕРЕВЬЯ:" << endl;
    cout << "  TCREATE <name>            - Создать дерево" << endl;
    cout << "  TINSERT <name> <value>    - Вставить элемент" << endl;
    cout << "  TDELETE <name> <value>    - Удалить элемент" << endl;
    cout << "  TSEARCH <name> <value>    - Найти элемент" << endl;
    
    cout << "\nОБЩИЕ КОМАНДЫ:" << endl;
    cout << "  PRINT <name>              - Показать содержимое структуры" << endl;
    cout << "  HELP                      - Справка" << endl;
    
    cout << "\nПРИМЕРЫ ЗАПУСКА:" << endl;
    cout << "  ./<program> --file data.txt --query 'TCREATE mytree'" << endl;
    cout << "  ./<program> --file data.txt --query 'TINSERT mytree 10'" << endl;
    cout << "  ./<program> --file data.txt --query 'PRINT mytree'" << endl;
    cout << "  ./<program> --help" << endl;
}