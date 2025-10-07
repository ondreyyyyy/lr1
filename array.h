#ifndef ARRAY_H
#define ARRAY_H

#include <string>

using namespace std;

struct DynArray{
    string* data;
    int size;
    int capacity;
};

void createArr(DynArray& arr, int capacity);
void addAtIndex(int index, string data, DynArray& arr);
void pushBackArr(string data, DynArray& arr);
string getElementIndex(int index, const DynArray& arr);
void removeFromIndex(int index, DynArray& arr);
void replacementElement(int index, string data, DynArray& arr);
int genLength(const DynArray& arr);
void printArray(const DynArray& arr);

#endif // ARRAY_H