#include <iostream>
#include <string>
#include "array.h"

using namespace std;

void createArr(DynArray& arr, int capacity) {
    arr.size = 0;
    arr.capacity = capacity;
    
    if (capacity > 0) {
        arr.data = new string[arr.capacity];
    } else {
        arr.data = nullptr;
    }
}

void addAtIndex(int index, string data, DynArray& arr) {
    if (index < 0 || index > arr.size) return;

    if (arr.capacity == arr.size) {
        int newCapacity = (arr.capacity == 0) ? 1 : arr.capacity * 2;
        string* tempArr = new string[newCapacity];

        for (int i = 0; i < index; i++) {
            tempArr[i] = arr.data[i]; 
        }

        tempArr[index] = data;

        for (int i = index; i < arr.size; i++) {
            tempArr[i + 1] = arr.data[i];
        }

        delete[] arr.data;
        arr.capacity = newCapacity;
        arr.data = tempArr;
    }
    else {
        for (int i = arr.size; i > index; i--) {
            arr.data[i] = arr.data[i - 1];
        }

        arr.data[index] = data;
    }

    arr.size++;
}

void pushBackArr(string data, DynArray& arr) {
    addAtIndex(arr.size, data, arr);
}

string getElementIndex(int index, const DynArray& arr) {
    if (index < 0 || index >= arr.size) throw out_of_range("Индекс не является элементом массива.");
    return arr.data[index];
}

void removeFromIndex(int index, DynArray& arr) {
    if (index < 0 || index >= arr.size) return;

    for(int i = index; i < arr.size - 1; i++) {
        arr.data[i] = arr.data[i + 1];
    }

    arr.size--;
}

void replacementElement(int index, string data, DynArray& arr) {
    if (index < 0 || index >= arr.size) return;
    arr.data[index] = data;
}

int genLength(const DynArray& arr) {
    return arr.size;
}

void printArray(const DynArray& arr) {
    if (arr.data == nullptr) {
        cout << "Массив пуст.\n";
        return;
    }

    for (int i = 0; i < arr.size; ++i) {
        cout << arr.data[i] << " ";
    }
    cout << "\nВсего элементов массива: " << arr.capacity <<
    "\nИз них заполнено: " << arr.size << endl;
}