//
//  Search.hpp
//  Project 3
//
//  Created by Christian Hackelman on 11/7/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

#ifndef SEARCH_H
#define SEARCH_H

#include "Exceptions.h"
#include "ResizableArray.h"
#include "Comparator.h"

// returns FIRST array location matching the given item (based on the comparator)
// if not found, returns -(location to insert + 1)
template <typename T>
long long binarySearch(const T& item, const ResizableArray<T>& array, const Comparator<T>& comparator) {
    // implementation goes here
    long long low = 0;
    long long ret = 0;
    long long high = (int)array.getSize() - 1;
    long long mid = (high + low)/2;
    
    while (high >= low) {
        if (comparator.compare(array.get(mid), item) == -1) {
            low = mid + 1;
        }
        else if (comparator.compare(array.get(mid), item) == 0){
            if ((unsigned long)mid == array.getSize()) {
                ret = mid -1;
                break;
            }
            ret = mid;
            break;
        }
        else {
            high = mid -1;
        }
        mid = (high + low)/2;
    }
    if (low > high) {
        return -1;
    }
    return ret;
}
template <typename T>
long long linearSearch(const T& item, int index, const ResizableArray<T>& array, const Comparator<T>& comparator) {
    for (unsigned int i = 0; i < array.getSize(); i++) {
        if (comparator.compare(item, array.get(i)) == 0) {
            return i;
        }
    }
    return -1;
}
#endif
