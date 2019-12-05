//
//  ResizableArray.hpp
//  Project 3
//
//  Created by Christian Hackelman on 11/7/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

#ifndef RESIZABLE_ARRAY_H
#define RESIZABLE_ARRAY_H

#include "Exceptions.h"

const unsigned long DEFAULT_ARRAY_CAPACITY = 10;        // capacity used in no arg constructor

template <typename T>
class ResizableArray {
private:
    unsigned long capacity = DEFAULT_ARRAY_CAPACITY;    // maximum capacity, in items
    unsigned long size = 0;                             // actual number of items currently in array
    T* data = NULL;                                     // pointer to array of any type
    void doubleCapacity();                              // method to double array capacity
    void halveCapacity();                               // method to halve array capacity
public:
    ResizableArray();                                   // constructs array with default capacity
    ResizableArray(unsigned long capacity);             // constructs array with specified capacity
    virtual ~ResizableArray();                          // frees array space as object is deleted
    void add(T item);                                   // adds item, increments size, doubles capacity as necessary
    void addAt(T item, unsigned long index);            // adds item at index, shifts following, doubles capacity as necessary
    void replaceAt(T item, unsigned long index);        // replaces item at index, otherwise unchanged
    void removeAt(unsigned long index);                 // removes item at index, shifts following back
    T get(unsigned long index) const;                   // returns (copy of) item at index
    unsigned long getSize() const;                      // returns number of items currently in array
    unsigned long getCapacity() const;                  // returns the current capacity of the array
    T operator[](unsigned long index) const;            // returns (copy of) item at index
    void clear();                                       // removes all items from array, resets to default capacity, set size to 0
};

// Add your implementation below this line. Do not add or modify anything above this line.

template <typename T>
ResizableArray<T>::ResizableArray() {
    data = new T[capacity];  // initializes array with default capacity
}

template <typename T>
ResizableArray<T>::ResizableArray(unsigned long capacity) {
    data = new T[capacity];                // initializes array with specified capacity
    this -> capacity = capacity;
}

template <typename T>
ResizableArray<T>::~ResizableArray<T>() {
    delete[] data;
}

template <typename T>
void ResizableArray<T>::add(T item) {
    if (size == capacity) {                  // check if the current array has space for another item
        doubleCapacity();
    }
    data[size] = item;                   // add item
    ++size;
}

template <typename T>
void ResizableArray<T>::addAt(T item, unsigned long index) {
    if (index > size) {
        throw new ExceptionIndexOutOfRange();
    }
    if (size == capacity) {
        doubleCapacity();
    }
    for (unsigned long j = size; j > index; --j) {
        data[j] = data[j - 1];
    }
    data[index] = item;                      // insert item into specified index
    ++size;
}

template <typename T>
void ResizableArray<T>::replaceAt(T item, unsigned long index) {
    if (index >= size) {
        throw new ExceptionIndexOutOfRange();
    }
    data[index] = item;
}

template <typename T>
void ResizableArray<T>::removeAt(unsigned long index) {
    if (index >= size) {
        throw new ExceptionIndexOutOfRange();
    }
    if (size == capacity / 2) {
        halveCapacity();
    }
    for (unsigned long i = index; i <= size -  1; ++i) {
        data[i] = data[i + 1];
    }                                             // shift items to the left starting with item to right of specified index
    --size;                                         // decrement size to account for item removal
    
}

template <typename T>
T ResizableArray<T>::get(unsigned long index) const {
    if (index >= size || index < 0) {
        throw new ExceptionIndexOutOfRange();
    }
    return data[index];
}
template <typename T>
T ResizableArray<T>::operator[](unsigned long index) const {
    if (index >= size) {
        throw new ExceptionIndexOutOfRange();
    }
    return data[index];
}

template <typename T>
unsigned long ResizableArray<T>::getSize() const {
    return size;
}

template <typename T>
unsigned long ResizableArray<T>::getCapacity() const {
    return capacity;
}

template <typename T>
void ResizableArray<T>::doubleCapacity() {
    unsigned long doublesize = capacity * 2;// double the capacity
    T* doublespace = new T[doublesize]; // create new array
    capacity = doublesize;
    for (unsigned long i = 0; i < size ; ++i) {
        doublespace[i] = data[i];
    } // copy over items from original array to new array
    
    delete[] data;
    data = doublespace;
}

template <typename T>
void ResizableArray<T>::halveCapacity() {
    unsigned long halfsize = capacity / 2;
    T* halfspace = new T[halfsize]; // create new array
    // half the capacity
    capacity = halfsize;
    for (unsigned long i = 0; i < halfsize; ++i) {
        halfspace[i] = data[i];
    }
    delete[] data;
    data = halfspace;                                 // half the size to account for capacity removal
}

template <typename T>
void ResizableArray<T>::clear() {
    delete[] data;
    T* newArray = new T[DEFAULT_ARRAY_CAPACITY];
    size = 0;
    capacity = DEFAULT_ARRAY_CAPACITY;
    data = newArray;
}
#endif /* ResizableArray_hpp */
