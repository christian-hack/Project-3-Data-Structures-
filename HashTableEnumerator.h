//
//  HashTableEnumerator.h
//  Project 3
//
//  Created by Christian Hackelman on 11/13/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

#ifndef HASH_TABLE_ENUMERATOR
#define HASH_TABLE_ENUMERATOR

#include "Enumerator.h"
#include "Exceptions.h"
#include "HashTable.h"

template <typename T>
class HashTableEnumerator : public Enumerator<T>
{
private:
    unsigned long bucket = 0;
    OULinkedListEnumerator<T>* chainEnumerator = NULL;
    HashTable<T>* hashTable;
public:
    HashTableEnumerator(HashTable<T>* hashTable);
    virtual ~HashTableEnumerator();
    bool hasNext() const;
    T next();              // throws ExceptionEnumerationBeyondEnd if no next item is available
    T peek() const;        // throws ExceptionEnumerationBeyondEnd if no next item is available
};

// Add your implementation below this line.
template <typename T>
HashTableEnumerator<T>::HashTableEnumerator(HashTable<T>* hashTable) {
    if (hashTable == nullptr || hashTable->getSize() == 0) {
        throw new ExceptionHashTableAccess;
    }
    this->hashTable = hashTable;
    while ((bucket < hashTable->getBaseCapacity() - 1) && (hashTable->table[bucket]->getSize() == 0)) {
        ++bucket;
    }
    chainEnumerator = new OULinkedListEnumerator<T>(hashTable->table[bucket]->enumerator());
}

template <typename T>
HashTableEnumerator<T>::~HashTableEnumerator<T>() {
    chainEnumerator = nullptr;
    bucket = 0;
}

template <typename T>
bool HashTableEnumerator<T>::hasNext() const {
    return chainEnumerator->hasNext();
}

template <typename T>
T HashTableEnumerator<T>::next() {
    if (!hasNext()) {
        throw new ExceptionEnumerationBeyondEnd;
    }
    T item = chainEnumerator->next();
    if (!chainEnumerator->hasNext()) {
        do {
            ++bucket;
        } while ((bucket < hashTable->getBaseCapacity()-1) && (hashTable->table[bucket]->getSize() == 0));
        if (bucket < hashTable->getBaseCapacity()) {
            chainEnumerator = new OULinkedListEnumerator<T>(hashTable->table[bucket]-> enumerator());
        }
    }
    return item;
}

template <typename T>
T HashTableEnumerator<T>::peek() const {
    return chainEnumerator->peek();
}
#endif // !HASH_TABLE_ENUMERATOR


