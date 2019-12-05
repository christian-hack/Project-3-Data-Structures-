//
//  OULinkedList.hpp
//  Project 3
//
//  Created by Christian Hackelman on 11/7/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

#ifndef OU_LINKED_LIST
#define OU_LINKED_LIST

#include "OULink.h"
#include "Comparator.h"
#include "OULinkedListEnumerator.h"

// OULinkedList stands for Ordered, Unique Linked List. It is a linked list that is always maintained in
// order (based on the comparator provided to it when the list is created) and that only contains unique
// items (that is, duplicates are not allowed)
template <typename T>
class OULinkedList {
    //template <typename T>
    //friend class OULinkedListEnumerator;
private:
    Comparator<T>* comparator = NULL;               // used to determine list order and item equality
    unsigned long size = 0;                         // actual number of items currently in list
    OULink<T>* first = NULL;                        // pointer to first link in list
    OULink<T>* last = NULL;                         // pointer to last link in list
public:
    OULinkedList(Comparator<T>* comparator);        // creates empty linked list with comparator
    virtual ~OULinkedList();                        // deletes all links and their data items
    
    // if an equivalent item is not already present, insert item in order and return true
    // if an equivalent item is already present, leave list unchanged and return false
    bool insert(T item);
    
    // if item is greater than item at last, append item at end and return true
    // if item is less than or equal to item at last, leave list unchanged and return false
    bool append(T item);
    
    // if an equivalent item is already present, replace item and return true
    // if an equivalent item is not already present, leave list unchanged and return false
    bool replace(T item);
    
    // if an equivalent item is already present, remove item and return true
    // if an equivalent item is not already present, leave list unchanged and return false
    bool remove(T item);
    
    // if any items are present, return a copy of the first item
    // if no items are present, throw new ExceptionLinkedListAccess
    T getFirst() const;
    
    // if any items are present, return a copy of the first item, remove it from list
    // if no items are present, throw new ExceptionLinkedListAccess
    T pullFirst();
    
    // if any items are present, remove the first item and return true
    // if no items are present, leave list unchanged and return false
    bool removeFirst();
    
    // if an equivalent item is present, return true
    // if an equivalent item is not present, false
    bool contains(T item) const;
    
    // if an equivalent item is present, return a copy of that item
    // if an equivalent item is not present, throw a new ExceptionLinkedListAccess
    T find(T item) const;
    
    void clear();                                     // deletes all links in the list, resets size to 0
    
    unsigned long getSize() const;                    // returns the current number of items in the list
    
    OULinkedListEnumerator<T> enumerator() const;     // create an enumerator for this linked list
};

// Add your implementation below this line. Do not add or modify anything above this line.

// creates empty linked list with comparator
template <typename T>
OULinkedList<T>::OULinkedList(Comparator<T>* comparator) {
    this->comparator = comparator;
}

// deletes all links and their data items
template <typename T>
OULinkedList<T>::~OULinkedList<T>(){
    delete first;
}

// if an equivalent item is not already present, insert item in order and return true
// if an equivalent item is already present, leave list unchanged and return false
template <typename T>
bool OULinkedList<T>::insert(T item) {
    // make sure the list isn't empty, if it is add item to first node's data
    if (size == 0) {
        OULink<T> *insertNode = new OULink<T>(item);
        first = insertNode;
        last = insertNode;
        ++size;
        return true;
    }
    if (size == 1) {
        OULink<T> *insertNode = new OULink<T>(item);
        if (comparator->compare(insertNode->data, first->data) == 1) {
            first->next = insertNode;
            last = insertNode;
            ++size;
            return true;
        }
        if (comparator->compare(insertNode->data, first->data) == -1) {
            insertNode->next = first;
            last = first;
            first = insertNode;
            ++size;
            return true;
        }
        if (comparator->compare(insertNode->data, first->data) == 0) {
            return false;
        }
    }
    OULink<T> *insertNode = new OULink<T>(item);
    OULink<T> *currentNode = first;
    OULink<T> *nextNode = first->next;
    if (comparator->compare(insertNode->data, last->data) == 1) {
        last->next = insertNode;
        last = insertNode;
        ++size;
        return true;
    }
    if (comparator->compare(insertNode->data, first->data) == -1) {
        insertNode->next = first;
        first = insertNode;
        ++size;
        return true;
    }
    while (currentNode->next != NULL) {
        if (comparator->compare(insertNode->data, currentNode->data) == 0) {
            return false;
        }
        else if (comparator->compare(insertNode->data, currentNode->data) == 1 && comparator->compare(insertNode->data, nextNode->data) == -1) {
            currentNode->next = insertNode;
            insertNode->next = nextNode;
            ++size;
            return true;
        }
        if (comparator->compare(currentNode->data, last->data) == 0) {
            last->next = insertNode;
            last = insertNode;
            ++size;
            return true;
        }
        currentNode = nextNode;
        nextNode = currentNode->next;
        
    }
    return false;
}

// if item is greater than item at last, append item at end and return true
// if item is less than or equal to item at last, leave list unchanged and return false
template <typename T>
bool OULinkedList<T>::append(T item) {
    bool listChanged = false;
    OULink<T> *endNode = new OULink<T>(item);
    if (first == nullptr) {
        first = endNode;
        last = endNode;
        ++size;
        return true;
    }
    if (comparator->compare(endNode->data, last->data) == 0 || comparator->compare(endNode->data, last->data) == -1) {
        return listChanged;
    }
    if (comparator->compare(item, last->data) == 1) {
        last->next = endNode;
        listChanged = true;
        ++size;
        last = endNode;
    }
    return listChanged;
}

// if an equivalent item is already present, replace item and return true
// if an equivalent item is not already present, leave list unchanged and return false
template <typename T>
bool OULinkedList<T>::replace(T item) {
    bool listChanged = false;
    OULink<T> *temp;
    temp = first;
    while (temp != NULL) {
        if (comparator->compare(item, temp->data) == 0) {
            temp->data = item;
            listChanged = true;
            break;
        }
        temp = temp->next;
    }
    return listChanged;
}

// if an equivalent item is already present, remove item and return true
// if an equivalent item is not already present, leave list unchanged and return false
template <typename T>
bool OULinkedList<T>::remove(T item) {
    bool listChanged = false;
    OULink<T> *temp = first;
    OULink<T> *nextNode;
    //OULink<T> *saveNode;
    if (temp == nullptr) {
        return false;
    }
    if (comparator->compare(temp->data, item) == 0) {
        first = first->next;
        temp->next = NULL;
        delete temp;
        temp = NULL;
        --size;
        return true;
    }
    while (temp->next != NULL) {
        nextNode = temp->next;
        if (comparator->compare(nextNode->data, item) == 0) {
            OULink<T> *saveNode = nextNode->next;
            temp->next = saveNode;
            nextNode->next = NULL;
            delete nextNode;
            nextNode = NULL;
            --size;
            return true;
        }
        temp = nextNode;
    }
    
    return listChanged;
}

// if any items are present, return a copy of the first item
// if no items are present, throw new ExceptionLinkedListAccess
template <typename T>
T OULinkedList<T>::getFirst() const {
    if (first == NULL) {
        throw new ExceptionLinkedListAccess;
    }
    return first->data;
}

// if any items are present, return a copy of the first item, remove it from list
// if no items are present, throw new ExceptionLinkedListAccess
template <typename T>
T OULinkedList<T>::pullFirst() {
    T dataCopy;
    if (first != NULL) {
        dataCopy = first->data;
        delete first;
        --size;
        return dataCopy;
    }
    throw new ExceptionLinkedListAccess;
}

// if any items are present, remove the first item and return true
// if no items are present, leave list unchanged and return false
template <typename T>
bool OULinkedList<T>::removeFirst() {
    bool listChanged = false;
    if (first == NULL) {
        return false;
    }
    if (first != NULL) {
        delete first;
        listChanged = true;
        --size;
    }
    return listChanged;
}

// if an equivalent item is present, return true
// if an equivalent item is not present, false
template <typename T>
bool OULinkedList<T>::contains(T item) const {
    // are we checking if the whole record matches or just the timestamp? If just the timestamp, how do I access that with only 'data'?
    bool itemFound = false;
    OULink<T> *temp = first;
    while (temp != NULL) {
        if (comparator->compare(item, temp->data) == 0) {
            itemFound = true;
        }
        temp = temp->next;
    }
    return itemFound;
}

// if an equivalent item is present, return a copy of that item
// if an equivalent item is not present, throw a new ExceptionLinkedListAccess
template <typename T>
T OULinkedList<T>::find(T item) const {
    OULink<T> *temp = first;
    OULink<T>* dataCopy = new OULink<T>(item);
    if (first == NULL) {
        throw new ExceptionLinkedListAccess;
    }
    while (temp != NULL) {
        if (comparator->compare(dataCopy->data, temp->data) == 0) {
            return temp->data;
        }
        temp = temp->next;
    }
    throw new ExceptionLinkedListAccess;
}

// deletes all links in the list, resets size to 0
template <typename T>
void OULinkedList<T>::clear() {
    OULink<T> *currentNode = first;
    OULink<T> *nextNode;
    while (currentNode != NULL) {
        currentNode->data = NULL;
        nextNode = currentNode->next;
        delete currentNode;
        currentNode = nextNode;
    }
}

// returns the current number of items in the list
template <typename T>
unsigned long OULinkedList<T>::getSize() const {
    return size;
}

// create an enumerator for this linked list
template <typename T>
OULinkedListEnumerator<T> OULinkedList<T>::enumerator() const {
    return OULinkedListEnumerator<T>(first);
}
#endif // !OU_LINKED_LIST
