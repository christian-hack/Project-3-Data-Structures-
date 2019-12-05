//
//  Sorter.hpp
//  Project 3
//
//  Created by Christian Hackelman on 11/7/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

#ifndef SORT_H
#define SORT_H

#include "Exceptions.h"
#include "ResizableArray.h"
#include "Comparator.h"

template <typename T>
class Sorter {
private:
    // additional member functions (methods) and variables (fields) may be added
    static void swap(int index, int secindex, ResizableArray<T>& array);
    static int partition(ResizableArray<T>& array, int left, int right, const Comparator<T>& comparator);
    static void quicksort(ResizableArray<T>& array, int left, int right, const Comparator<T>& comparator);
    
public:
    static void sort(ResizableArray<T>& array, const Comparator<T>& comparator);
    
};

template <typename T>
void Sorter<T>::swap(int index, int secIndex, ResizableArray<T> &array) {
    T temp = array[index];
    array.replaceAt(array[secIndex], index);
    array.replaceAt(temp, secIndex);
}

template <typename T>
int Sorter<T>::partition(ResizableArray<T> &array, int left, int right, const Comparator<T> &comparator) {
    int index = left;
    int pivot = right;
    
    for (int i = index; i < right; i++) {
        if (comparator.compare(array[i], array[pivot]) == -1) {
            swap(i, index, array);
            index++;
        }
    }
    swap(pivot, index, array);
    return index;
}

template <typename T>
void Sorter<T>::quicksort(ResizableArray<T> &array, int left, int right, const Comparator<T>& comparator) {
    int pindex;
    if (left < right) {
        pindex = partition(array, left, right, comparator);
        quicksort(array, left, pindex-1, comparator);
        quicksort(array, pindex+1, right, comparator);
    }
}

template <typename T>
void Sorter<T>::sort(ResizableArray<T> &array, const Comparator<T> &comparator) {
    
    int low = 0;
    int high = (int)array.getSize() - 1;
    
    quicksort(array, low, high, comparator);
}
#endif


