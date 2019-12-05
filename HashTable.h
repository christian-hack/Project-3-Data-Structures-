//
//  HashTable.hpp
//  Project 3
//
//  Created by Christian Hackelman on 11/7/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

#ifndef HASH_TABLE
#define HASH_TABLE

#include <math.h>
#include "Exceptions.h"
#include "Comparator.h"
#include "OULinkedList.h"
#include "OULinkedListEnumerator.h"
#include "Hasher.h"

const unsigned int SCHEDULE_SIZE = 25;
const unsigned int SCHEDULE[SCHEDULE_SIZE] = { 1, 2, 5, 11, 23, 53, 107, 223, 449, 907, 1823, 3659, 7309, 14621, 29243, 58511, 117023, 234067, 468157, 936319, 1872667, 3745283, 7490573, 14981147, 29962343 };
const unsigned int DEFAULT_SCHEDULE_INDEX = 3;
const unsigned long DEFAULT_BASE_CAPACITY = SCHEDULE[DEFAULT_SCHEDULE_INDEX];
const float DEFAULT_MAX_LOAD_FACTOR = 0.8f;
const float DEFAULT_MIN_LOAD_FACTOR = 0.3f;

template <typename T>
class HashTable {
    template <typename U>
    friend class HashTableEnumerator;
private:
    Comparator<T>* comparator;                                // used to determine item equality
    Hasher<T>* hasher;                                        // used to compute hash value
    unsigned long size = 0;                                    // actual number of items currently in hash table
    float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR;
    float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR;
    unsigned int scheduleIndex = DEFAULT_SCHEDULE_INDEX;
    unsigned long baseCapacity = DEFAULT_BASE_CAPACITY;        // the size of the array
    unsigned long totalCapacity = baseCapacity;                // the size of the array plus chains of more than one link
    OULinkedList<T>** table = NULL;                            // table will be an array of pointers to OULinkedLists of type T
    // you may add additional member variables and functions here to support the operation of your code
public:
    HashTable(Comparator<T>* comparator, Hasher<T>* hasher);            // creates an empty table of DEFAULT_BASE_CAPACITY
    HashTable(Comparator<T>* comparator, Hasher<T>* hasher,
              // if size given, creates empty table with size from schedule of sufficient capacity (considering maxLoadFactor)
              unsigned long size,
              float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR,
              float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR);
    virtual ~HashTable();
    
    // if an equivalent item is not already present, insert item at proper location and return true
    // if an equivalent item is already present, leave table unchanged and return false
    bool insert(T item);
    
    // if an equivalent item is already present, replace item and return true
    // if an equivalent item is not already present, leave table unchanged and return false
    bool replace(T item);
    
    // if an equivalent item is already present, remove item and return true
    // if an equivalent item is not already present, leave table unchanged and return false
    bool remove(T item);
    
    // if an equivalent item is present, return a copy of the item
    // if an equivalent item is not present, throw a new ExceptionHashTableAccess
    T find(T item) const;
    
    unsigned long getSize() const;                            // returns the current number of items in the table
    unsigned long getBaseCapacity() const;                    // returns the current base capacity of the table
    unsigned long getTotalCapacity() const;                    // returns the current total capacity of the table
    float getLoadFactor() const;                            // returns the current load factor of the table
    unsigned long getBucketNumber(T item) const;            // returns the current bucket number for an item
};

// Add your implementation below this line.

template <typename T>
HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher) {
    table = new OULinkedList<T>*[baseCapacity];
    this->comparator = comparator;
    this->hasher = hasher;
    for (unsigned int i = 0; i < DEFAULT_BASE_CAPACITY; ++i) {
        table[i] = new OULinkedList<T>(comparator);
    }
}

template <typename T>
HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher, unsigned long size, float maxLoadFactor, float minLoadFactor) {
    // loop through the schedule array to find an instance in which the desired size is less than the value at a given index of the SCHEDULE array
    // then create an array based off the the appropriate index
    this->minLoadFactor = minLoadFactor;
    this->maxLoadFactor = maxLoadFactor;
    this->comparator = comparator;
    this->hasher = hasher;
    
    float bucket = (float)size/maxLoadFactor;
    for (scheduleIndex = 0; scheduleIndex < SCHEDULE_SIZE; ++scheduleIndex) {
        if (bucket < (float) SCHEDULE[scheduleIndex]) {
            baseCapacity = SCHEDULE[scheduleIndex];
            totalCapacity = baseCapacity;
            table = new OULinkedList<T>*[baseCapacity];
            for (unsigned int j = 0; j < baseCapacity; ++j) {
                table[j] = new OULinkedList<T>(comparator);
            }
            break;
        }
        
    }
}

template <typename T>
HashTable<T>::~HashTable<T>() {
    delete[] table;
}

template <typename T>
bool HashTable<T>::insert(T item) {
    unsigned long bucket = getBucketNumber(item);
    bool isInserted = false;
    if (table[bucket]->insert(item)) {
        isInserted = true;
        ++size;
        if (table[bucket] -> getSize() > 1) {
            ++totalCapacity;
        }
    }
    else {
        isInserted = false;
    }
    // check to see if load factor has or will been reached
    // if ((float)size + 1 == (float)totalCapacity * maxLoadFactor) {
    if (getLoadFactor() > maxLoadFactor) {
        // if load factor has or will be reached, create resized hash table
        unsigned long oldBaseCapacity = baseCapacity;
        OULinkedList<T>** oldTable = table;
        ++scheduleIndex;
        baseCapacity = SCHEDULE[scheduleIndex];
        totalCapacity = baseCapacity;
        table = new OULinkedList<T>*[baseCapacity];
        // copy over items from old has table to newly resized hash table
        for (unsigned int i = 0; i < baseCapacity; ++i) {
            table[i] = new OULinkedList<T>(comparator);
        }
        for (unsigned int i = 0; i < oldBaseCapacity; ++i) {
            OULinkedListEnumerator<T> e = oldTable[i] -> enumerator();
            while (e.hasNext()) {
                bucket = getBucketNumber(e.peek()); //hasher->hash(e.peek()) % baseCapacity;
                table[bucket]->insert(e.next());
                if (table[bucket]->getSize() > 1) {
                    ++totalCapacity;
                }
            }
        }
        delete[] oldTable;
    }
    return isInserted;
}

template <typename T>
bool HashTable<T>::replace(T item) {
    unsigned long bucket = getBucketNumber(item);
    return table[bucket] -> replace(item);
}

template <typename T>
bool HashTable<T>::remove(T item) {
    unsigned long bucket = getBucketNumber(item);
    if (table[bucket] -> remove(item)) {
        if (table[bucket] -> getSize() > 0) {
            --totalCapacity;
        }
        --size;
        unsigned long newLoad = size/totalCapacity;
        if (newLoad < (unsigned long)minLoadFactor) {
            unsigned long oldBaseCapacity = baseCapacity;
            OULinkedList<T>** oldTable = table;
            --scheduleIndex;
            baseCapacity = SCHEDULE[scheduleIndex];
            totalCapacity = baseCapacity;
            table = new OULinkedList<T>*[baseCapacity];
            
            for (unsigned int i = 0; i < baseCapacity; ++i) {
                table[i] = new OULinkedList<T>(comparator);
            }
            for (unsigned int i = 0; i < oldBaseCapacity; ++i) {
                OULinkedListEnumerator<T> e = oldTable[i] -> enumerator();
                while (e.hasNext()) {
                    bucket = getBucketNumber(e.peek()); //hasher->hash(e.peek()) % baseCapacity;
                    table[bucket]->insert(e.next());
                    if (table[bucket]->getSize() > 1) {
                        ++totalCapacity;
                    }
                }
            }
        }
        return true;
    }
    return false;
}

template <typename T>
T HashTable<T>::find(T item) const {
    unsigned long bucket = hasher->hash(item) % baseCapacity;
    if (table[bucket]->contains(item)) {
        T record = table[bucket]->find(item);
        return record;
    }
    throw new ExceptionHashTableAccess;
}

template <typename T>
unsigned long HashTable<T>::getSize() const {
    return size;
}

template <typename T>
unsigned long HashTable<T>::getBaseCapacity() const {
    return baseCapacity;
}

template <typename T>
unsigned long HashTable<T>::getTotalCapacity() const {
    return totalCapacity;
}

template <typename T>
float HashTable<T>::getLoadFactor() const {
    return (float)size/(float)totalCapacity;
}

template <typename T>
unsigned long HashTable<T>::getBucketNumber(T item) const {
    unsigned long bucket = hasher->hash(item) % baseCapacity;
    return bucket;
}
#endif // !HASH_TABLE
