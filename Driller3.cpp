//
//  Driller3.cpp
//  Project 3
//
//  Created by Christian Hackelman on 11/7/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "ResizableArray.h"
#include "Enumerator.h"
#include "OULink.h"
#include "OULinkedList.h"
#include "OULinkedListEnumerator.h"
#include "DrillingRecord.h"
#include "DrillingRecordComparator.h"
#include "Sorter.h"
#include "Search.h"
#include "Comparator.h"
#include "DrillingRecordHasher.h"
#include "Hasher.h"
#include "HashTable.h"
#include "HashTableEnumerator.h"
using namespace std;
HashTable<DrillingRecord>* listToHashArray (ResizableArray<DrillingRecord>& array, OULinkedList<DrillingRecord>& linkedList,HashTable<DrillingRecord>* hashTable, DrillingRecordComparator* drc, Hasher<DrillingRecord>* hasher) {
    array.clear();
    delete hashTable;
    hashTable = new HashTable<DrillingRecord>(drc, hasher, linkedList.getSize());
    OULinkedListEnumerator<DrillingRecord> listEnumerator = linkedList.enumerator();
    if (linkedList.getSize() != 0) {
        while (listEnumerator.hasNext()) {
            hashTable->insert(listEnumerator.peek());
            array.add(listEnumerator.next());
        }
    }
    return hashTable;
}
void mergeDataManipulation (ResizableArray<DrillingRecord>& array, int& lineCounter, string date, int& validRecords, vector<string>& timestamps, int& validRecordsInMem, OULinkedList<DrillingRecord>& linkedList, HashTable<DrillingRecord>* hashTable, DrillingRecordComparator* drc, Hasher<DrillingRecord>* hasher) {
    string line;
    string tempDate;
    string timeColumnValue;
    vector<double> floatValues;
    string floatValue;
    string input;
    bool invalidDateFound = false;
    bool invalidFloatValueFound = false;
    bool lineIsValid = true;
    bool timeMatchFound = false;
    // int timeMatchIndex = -1;
    cout << "Enter data file name: ";
    getline(cin, input);
    // not checking for "ENTER"?
    if (input.size() == 0) {
        //cout << "Only one file entered" << endl;
        return;
    }
    else {
        //where subsequent files would be read maybe
        ifstream ifs(input);
        //ifs.open(input);
        if (ifs.is_open()) {
            getline(ifs, line);
            //getline(ifs, line);
            //++lineCounter;
            while(getline(ifs, line)) {
                DrillingRecord drillRec;
                ++lineCounter;
                stringstream ss(line);
                getline(ss, tempDate, ',');
                if (tempDate.compare(date) != 0) {
                    invalidDateFound = true;
                    break;
                }
                getline(ss, timeColumnValue, ',');
                for (int i = 0; i < 16; ++i) {
                    getline(ss, floatValue, ',');
                    floatValues.push_back(stod(floatValue));
                }
                for (int j = 0; j < 16; ++j) {
                    if (floatValues[j] <= 0) {
                        invalidFloatValueFound = true;
                    }
                }
                if (invalidFloatValueFound == true) {
                    cout << "Invalid floating-point data at line " << lineCounter << "." << endl;
                    floatValues.clear();
                    lineIsValid = false;
                }
                for (unsigned int k = 0; k < timestamps.size(); ++k) {
                    if (timestamps[k] == timeColumnValue) {
                        timeMatchFound = true;
                        //timeMatchIndex = k;
                    }
                }
                if (lineIsValid == true && timeMatchFound == false) {
                    timestamps.push_back(timeColumnValue);
                }
                if (invalidFloatValueFound == false && lineIsValid == true && timeMatchFound == false) {
                    drillRec.addString(tempDate);
                    drillRec.addString(timeColumnValue);
                    for (int o = 0; o < 16; ++o) {
                        drillRec.addNum(floatValues[o]);
                    }
                    linkedList.insert(drillRec);
                    ++validRecords;
                    ++validRecordsInMem;
                    floatValues.clear();
                }
                if (invalidFloatValueFound == false && lineIsValid == true && timeMatchFound == true) {
                    drillRec.addString(tempDate);
                    drillRec.addString(timeColumnValue);
                    for (int p = 0; p < 16; ++p) {
                        drillRec.addNum(floatValues[p]);
                    }
                    if (linkedList.contains(drillRec)) {
                        linkedList.replace(drillRec);
                    }
                    linkedList.insert(drillRec);
                    floatValues.clear();
                    timeMatchFound = false;
                    ++validRecords;
                }
                if (invalidFloatValueFound == true && timeMatchFound == true) {
                    invalidFloatValueFound = false;
                    lineIsValid = true;
                    timeMatchFound = false;
                }
                if (invalidFloatValueFound == true && timeMatchFound == false) {
                    invalidFloatValueFound = false;
                    lineIsValid = true;
                }
            }
            if (invalidDateFound == true) {
                cout << "Date mismatch; file closed.";
                invalidDateFound = false;
                mergeDataManipulation(array, lineCounter, date, validRecords, timestamps, validRecordsInMem, linkedList, hashTable, drc, hasher);
            }
            //listToArray(linkedList, array);
            //listToHashArray(array, linkedList, hashTable, drc, hasher);
        }
        else {
            cout << "File is not available." << endl;
            mergeDataManipulation(array, lineCounter, date, validRecords, timestamps, validRecordsInMem, linkedList, hashTable, drc, hasher);
        }
    }
    // mergeDataManipulation(array, lineCounter, date, validRecords, timestamps, validRecordsInMem, linkedList);
}
void readFile(ResizableArray<DrillingRecord>& array, bool& dataWasEntered, int& lineCounter, string& date, int& validRecords, int& validRecordsInMem, OULinkedList<DrillingRecord>& linkedList, vector<string>& timestamps) {
    string input;
    string input2;
    string line;
    vector<double> floatvalues;
    string timeColumnValue;
    string floatValue;
    string tempDate;
    bool validFirstLineFound = false;
    bool timeMatchFound = false;
    bool lineIsValid = true;
    bool firstLineRead = false;
    bool invalidFloatValue = false;
    bool invalidFloatValue2 = false;
    //bool filesBeingEntered = true;
    cout << "Enter data file name: ";
    getline(cin, input);
    if (input.size() != 0) {
        // if user entered something, execute
        ifstream ifs(input);
        //ifs.open(input);
        if (ifs.is_open()) {
            // we can actually begin to read in the file
            // read in header line
            getline(ifs, line);
            getline(ifs, line);
            while (firstLineRead == false) {
                ++lineCounter;
                //create DrillingRecord object for first line read in overall
                DrillingRecord originalDrillRec;
                // split first line of data into separate strings
                stringstream ss(line);
                // assign date reference variable to first date of original file
                getline(ss, date, ',');
                getline(ss, timeColumnValue, ',');
                // add first timestamp to vector
                timestamps.push_back(timeColumnValue);
                // enter loop to assign the rest of line to floatValue vector
                for (int i = 0; i < 16; ++i) {
                    getline(ss, floatValue, ',');
                    floatvalues.push_back(stod(floatValue));
                }
                // enter loop to check for any invalid float values
                for (int j = 0; j < 16; ++j) {
                    if (floatvalues[j] <= 0) {
                        invalidFloatValue = true;
                    }
                }
                // if an invalid float value is found clear all variables, vectors
                if (invalidFloatValue == true) {
                    // report error to user
                    cout << "Invalid floating-point data at line " << lineCounter << "." << endl;
                    timestamps.clear();
                    floatvalues.clear();
                    invalidFloatValue = false;
                    firstLineRead = true;
                }
                else {
                    // if line is completely valid, add into drillingrecord and add to array
                    originalDrillRec.addString(date);
                    originalDrillRec.addString(timeColumnValue);
                    for (int k = 0; k < 16; ++k) {
                        originalDrillRec.addNum(floatvalues[k]);
                    }
                    linkedList.insert(originalDrillRec);
                    ++validRecords;
                    ++validRecordsInMem;
                    firstLineRead = true;
                    floatvalues.clear();
                }
            }
            while (getline(ifs, line)) {
                ++lineCounter;
                stringstream ss2(line);
                DrillingRecord drillRec;
                getline(ss2, tempDate, ',');
                getline(ss2, timeColumnValue, ',');
                // enter loop to add float values to vector
                for (int m = 0; m < 16; ++m) {
                    getline(ss2, floatValue, ',');
                    floatvalues.push_back(stod(floatValue));
                }
                // enter loop to check for invalid float values
                for (int n = 0; n < 16; ++n) {
                    if (floatvalues[n] <= 0) {
                        invalidFloatValue2 = true;
                    }
                }
                // if invalid float value is found, execute
                if (invalidFloatValue2 == true) {
                    cout << "Invalid floating-point data at line " << lineCounter << "." << endl;
                    floatvalues.clear();
                    lineIsValid = false;
                }
                // enter loop to check for matching timestamps
                for (unsigned int l = 0; l < timestamps.size(); ++l) {
                    if (timestamps[l].compare(timeColumnValue) == 0) {
                        timeMatchFound = true;
                    }
                }
                // add time to timestamp vector if the line is valid and no timestamp match was found
                if (lineIsValid == true && timeMatchFound == false) {
                    timestamps.push_back(timeColumnValue);
                }
                // if line is valid and no timestamp match was found, execute
                if (invalidFloatValue2 == false && lineIsValid == true && timeMatchFound == false) {
                    drillRec.addString(tempDate);
                    drillRec.addString(timeColumnValue);
                    for (int o = 0; o < 16; ++o) {
                        drillRec.addNum(floatvalues[o]);
                    }
                    linkedList.insert(drillRec);
                    ++validRecords;
                    ++validRecordsInMem;
                    floatvalues.clear();
                    if (validFirstLineFound == false) {
                        validFirstLineFound = true;
                        date = tempDate;
                    }
                }
                if (invalidFloatValue2 == false && lineIsValid == true && timeMatchFound == true) {
                    drillRec.addString(tempDate);
                    drillRec.addString(timeColumnValue);
                    for (int p = 0; p < 16; ++p) {
                        drillRec.addNum(floatvalues[p]);
                    }
                    linkedList.replace(drillRec);
                    floatvalues.clear();
                    timeMatchFound = false;
                }
                if (invalidFloatValue2 == true && timeMatchFound == true) {
                    invalidFloatValue2 = false;
                    lineIsValid = true;
                    timeMatchFound = false;
                    timestamps.clear();
                }
                if (invalidFloatValue2 == true && timeMatchFound == false) {
                    invalidFloatValue2 = false;
                    lineIsValid = true;
                    timestamps.clear();
                }
            }
            //listToHashArray(array, linkedList, drc, hashTable);
            dataWasEntered = true;
            //end of file reading
        }
        else {
            cout << "File is not available." << endl;
            //call to read "first file" again
            readFile(array, dataWasEntered, lineCounter, date, validRecords, validRecordsInMem, linkedList, timestamps);
        }
    }
    else {
        dataWasEntered = false;
    }
}
void outputDataManipulation(int validCount, ResizableArray<DrillingRecord>* array, int totalLines, int recordsInMemory) {
    string input;
    cout << "Enter output file name: ";
    getline(cin, input);
    if (input.size() != 0) {                                                // if user enters a file name, execute
        ofstream ofs(input);
        if (ofs.is_open()) {                                                // if file is valid, execute
            if (array->getSize() != 0) {
                for (unsigned int i = 0; i <= array->getSize()-1; ++i) {
                    ofs << array->get(i) << endl;
                }
                ofs << "Data lines read: " << totalLines << "; Valid Drilling records read: " << validCount << "; Drilling records in memory: " << recordsInMemory << endl;
            }
            else {
                ofs << "Data lines read: " << totalLines << "; Valid Drilling records read: " << validCount << "; Drilling records in memory: " << recordsInMemory << endl;
            }
        }
        else {
            cout << "File is not available." << endl;
            outputDataManipulation(validCount, array, totalLines, recordsInMemory);
        }
    }
    else {
        // if user hits "ENTER", output data using standard cout
        if (array->getSize() != 0) {
            for (unsigned int i = 0; i <= array->getSize()-1; ++i) {
                cout << array->get(i) << endl;
            }
            cout << "Data lines read: " << totalLines << "; Valid Drilling records read: " << validCount << "; Drilling records in memory: " << recordsInMemory << endl;
        }
        else {
            cout << "Data lines read: " << totalLines << "; Valid Drilling records read: " << validCount << "; Drilling records in memory: " << recordsInMemory << endl;
        }
    }
}
void sortingDataManipulation(ResizableArray<DrillingRecord>& array, int& sortedColumn) {
    string input;
    cout << "Enter sort field (0-17): ";
    getline(cin, input);
    int column = stoi(input);
    if (column < 0 || column > 17) {
        // if user enters invalid number exit function
        return;
    }
    else {
        // if user enter valid number, execute
        if (column < 18 && column > 0) {
            DrillingRecordComparator* drc = new DrillingRecordComparator(column);
            Sorter<DrillingRecord>::sort(array, *drc);
            sortedColumn = column;
        }
    }
}
void findDataManipulation(ResizableArray<DrillingRecord>* array, int sortedCol) {
    string input;
    int recordsFound = 0;
    int index = -1;
    cout << "Enter search field (0-17): ";
    getline(cin, input);
    if (input.size() == 0) {
        return;
    }
    int field = stoi(input);
    double fieldspec = 0.0;
    if (field < 0 || field > 17) {
        return;
    }
    if (field > 1) {
        input.clear();
        cout << "Enter positive field value: ";
        getline(cin, input);
        if (input.size() == 0) {
            return;
        }
        try {
            fieldspec = stod(input);
        } catch (const exception e) {
            fieldspec = -1;
        }
        if (fieldspec > 0) {
            DrillingRecord drNum = DrillingRecord();
            drNum.setNum(fieldspec, field);
            DrillingRecordComparator* drc = new DrillingRecordComparator(field);
            if (field == sortedCol) {
                index = (int)binarySearch(drNum, *array, *drc);
                if (index >= 0) {
                    while (array->get(index).getNum(field) == drNum.getNum(field)) {
                        cout << array -> get(index) << endl;
                        ++recordsFound;
                        if ((unsigned long)index+1 == array->getSize()) {
                            break;
                        }
                        ++index;
                    }
                    cout << "Drilling records found: " << recordsFound << "." << endl;
                }
            }
            else {
                for (unsigned int i = 0; i < array->getSize(); ++i) {
                    if (array->get(i).getNum(field - 2) == fieldspec) {
                        cout << array->get(i) << endl;
                        ++recordsFound;
                    }
                }
                cout << "Drilling records found: " << recordsFound << "." << endl;
            }
        }
        if (fieldspec < 0) {
            cout << "Drilling records found: 0.";
            return;
        }
    }
    if (field < 2) {
        input.clear();
        cout << "Enter exact text on which to sort: ";
        getline(cin, input);
        if (input.size() == 0) {
            return;
        }
        DrillingRecord drString = DrillingRecord();
        drString.setString(input, field);
        DrillingRecordComparator* drc2 = new DrillingRecordComparator(field);
        if (field == sortedCol) {
            index = (int)binarySearch(drString, *array, *drc2);
            if (index >= 0) {
                while (array->get(index).getString(field) == drString.getString(field)) {
                    cout << array -> get(index) << endl;
                    ++recordsFound;
                    if ((unsigned long)index+1 == array->getSize()) {
                        break;
                    }
                    ++index;
                }
            }
            cout << "Drilling records found: " << recordsFound << "." << endl;
        }
        else {
            for (unsigned int i = 0; i < array->getSize(); ++i) {
                if (array->get(i).getString(field) == input) {
                    cout << array->get(i) << endl;
                    ++recordsFound;
                }
            }
            cout << "Drilling records found: " << recordsFound << "." << endl;
        }
    }
}
void purgeDataManipulation(OULinkedList<DrillingRecord>& linkedList, int& lineCounter, string date, int& validRecords, vector<string>& timestamps, int& validRecordsInMem, ResizableArray<DrillingRecord>& array, HashTable<DrillingRecord>* hashTable, DrillingRecordComparator* drc, Hasher<DrillingRecord>* hasher) {
    string line;
    string tempDate;
    string timeColumnValue;
    vector<double> floatValues;
    string floatValue;
    string input;
    bool invalidDateFound = false;
    bool invalidFloatValueFound = false;
    bool lineIsValid = true;
    bool timeMatchFound = false;
    int lineCount = 0;
    int timeMatchIndex = -1;
    cout << "Enter data file name: ";
    getline(cin, input);
    // not checking for "ENTER"?
    if (input.size() != 0) {
        ifstream ifs(input);
        if (ifs.is_open()) {
            getline(ifs, line);
            while (getline(ifs, line)) {
                DrillingRecord drillRec;
                ++lineCounter;
                ++lineCount;
                stringstream ss(line);
                getline(ss, tempDate, ',');
                if (tempDate.compare(date) != 0) {
                    invalidDateFound = true;
                    break;
                }
                getline(ss, timeColumnValue, ',');
                for (int i = 0; i < 16; ++i) {
                    getline(ss, floatValue, ',');
                    floatValues.push_back(stod(floatValue));
                }
                for (int j = 0; j < 16; ++j) {
                    if (floatValues[j] <= 0) {
                        invalidFloatValueFound = true;
                    }
                }
                if (invalidFloatValueFound == true) {
                    cout << "Invalid float value found at line " << lineCount << endl;
                    floatValues.clear();
                    lineIsValid = false;
                }
                for (unsigned int k = 0; k < timestamps.size(); ++k) {
                    if (timestamps[k] == timeColumnValue) {
                        timeMatchFound = true;
                        timeMatchIndex = k;
                    }
                }
                if (timeMatchFound == true && lineIsValid == true && invalidDateFound == false && invalidFloatValueFound == false) {
                    drillRec.addString(tempDate);
                    drillRec.addString(timeColumnValue);
                    ++validRecords;
                    //DrillingRecordComparator* drc = new DrillingRecordComparator(1);
                    for (int p = 0; p < 16; ++p) {
                        drillRec.addNum(floatValues[p]);
                    }
                    if (linkedList.remove(drillRec) == true) {
                        --validRecordsInMem;
                        timestamps.at(timeMatchIndex) = "";
                    }
                    floatValues.clear();
                }
                if (timeMatchFound == false && lineIsValid == true && invalidDateFound == false && invalidFloatValueFound == false) {
                    ++validRecords;
                    floatValues.clear();
                }
                invalidFloatValueFound = false;
                lineIsValid = true;
                timeMatchFound = false;
                if (invalidDateFound == true) {
                    cout << "Date mismatch; file closed." << endl;
                    purgeDataManipulation(linkedList, lineCounter, date, validRecords, timestamps, validRecordsInMem, array, hashTable, drc, hasher);
                }
                //end of while
            }
            //listToHashArray(array, linkedList, hashTable, drc, hasher);
            //listToArray(linkedList, array);
        }
        else {
            cout << "File is not available." << endl;
            purgeDataManipulation(linkedList, lineCounter, date, validRecords, timestamps, validRecordsInMem, array, hashTable, drc, hasher);
        }
    }
    else {
        return;
    }
}
void recordsDataManipulation(OULinkedList<DrillingRecord>* linkedList, int totalLines, int validCount, int recordsInMemory) {
    string input;
    cout << "Enter output file name: ";
    getline(cin, input);
    //OULinkedListEnumerator<DrillingRecord> enum;
    if (input.size() != 0) {
        // if user enters a file name, execute
        ofstream ofs(input);
        if (ofs.is_open()) {
            // if file is valid, execute
            OULinkedListEnumerator<DrillingRecord> e = linkedList->enumerator();
            //ofs << linkedList->getFirst() << endl;
            while (e.hasNext()) {
                ofs << e.next() << endl;
            }
            ofs << e.peek() << endl;
            ofs << "Data lines read: " << totalLines << "; Valid Drilling records read: " << validCount << "; Drilling records in memory: " << recordsInMemory << endl;
        }
        else {
            cout << "File is not available." << endl;
            recordsDataManipulation(linkedList, totalLines, validCount, recordsInMemory);
        }
    }
    else {
        // if user hits "ENTER", output data using standard cout
        OULinkedListEnumerator<DrillingRecord> e = linkedList->enumerator();
        while (e.hasNext()) {
            cout << e.next() << endl;
        }
        cout << e.peek() << endl;
        cout << "Data lines read: " << totalLines << "; Valid Drilling records read: " << validCount << "; Drilling records in memory: " << recordsInMemory << endl;
    }
}
 void hashTableDataManipulation(DrillingRecordComparator drc, HashTable<DrillingRecord>* hashTable, int totalLines, int validCount, int recordsInMemory) {
     HashTableEnumerator<DrillingRecord> enumerator(hashTable);
     string input;
     unsigned long currBucket = 0;
     //unsigned long currBucketSize = 0;
     cout << "Enter output file name: " << endl;
     getline(cin, input);
    if (input.size() != 0) {
         ofstream ofs(input);
         if (ofs.is_open()) {
             
             while (enumerator.hasNext()) {
                 currBucket = hashTable->getBucketNumber(enumerator.peek());
                 ofs << currBucket << ": " << enumerator.next() << endl;
                 while (enumerator.hasNext() && currBucket == hashTable->getBucketNumber(enumerator.peek())) {
                     ofs << "OVERFLOW: " << enumerator.next() << endl;
                 }
                 ofs << endl;
             }
             //ofs << enumerator->peek() << endl;
             ofs << "Base Capacity: " << hashTable->getBaseCapacity() << "; Total Capacity: " << hashTable->getTotalCapacity() << "; Load Factor: " << hashTable->getLoadFactor() << endl;
             ofs << "Data lines read: " << totalLines << "; Valid Drilling records read: " << validCount << "; Drilling records in memory: " << recordsInMemory << endl;
         }
         else {
             cout << "File is not available." << endl;
             hashTableDataManipulation(drc, hashTable, totalLines, validCount, recordsInMemory);
         }
     }
    else {
        while (enumerator.hasNext()) {
            currBucket = hashTable->getBucketNumber(enumerator.peek());
            cout << currBucket << ": " << enumerator.next() << endl;
            /*
            if (!enumerator->hasNext()) {
                break;
            }
             */
            while (enumerator.hasNext() && currBucket == hashTable->getBucketNumber(enumerator.peek())) {
                cout << "OVERFLOW: " << enumerator.next() << endl;
            }
            cout << endl;
        }
        //cout << enumerator->peek() << endl;
        cout << "Base Capacity: " << hashTable->getBaseCapacity() << "; Total Capacity: " << hashTable->getTotalCapacity() << "; Load Factor: " << hashTable->getLoadFactor() << endl;
        cout << "Data lines read: " << totalLines << "; Valid Drilling records read: " << validCount << "; Drilling records in memory: " << recordsInMemory << endl;
    }
 }
int main() {
    /*
    DrillingRecord d1;
    d1.setString("1", 1);
    
    DrillingRecord d2;
    d2.setString("2", 1);
    
    DrillingRecordComparator * drc = new DrillingRecordComparator(1);
    DrillingRecordHasher * drh = new DrillingRecordHasher();
    
    HashTable<DrillingRecord> * ht = new HashTable<DrillingRecord>(drc, drh);
    ht->insert(d1);
    ht->insert(d2);
    HashTableEnumerator<DrillingRecord> * hte = new HashTableEnumerator<DrillingRecord>(ht);
    
    while(hte->hasNext()) {
        hte->next();
    }
    */
    
     string input = " ";
     int lineCount = 0;
     string date;
     vector<string> timestamps;
     int validRecords = 0;
     int validRecordsInMem = 0;
     bool dataWasEntered = true;
     int sortedCol = 1;
     unsigned int linkedListSort = 1;
     ResizableArray<DrillingRecord>* resizedArray = new ResizableArray<DrillingRecord>();
     DrillingRecordComparator* drc = new DrillingRecordComparator(linkedListSort);
     OULinkedList<DrillingRecord>* linkedList = new OULinkedList<DrillingRecord>(drc);
     Hasher<DrillingRecord>* hasher = new DrillingRecordHasher();
     //HashTable<DrillingRecord>* hashTable = new HashTable<DrillingRecord>(drc, hasher);
     readFile(*resizedArray, dataWasEntered, lineCount, date, validRecords, validRecordsInMem, *linkedList, timestamps);
     HashTable<DrillingRecord>* hashTable = new HashTable<DrillingRecord>(drc, hasher, linkedList->getSize());
     hashTable = listToHashArray(*resizedArray, *linkedList, hashTable, drc, hasher);
     if (dataWasEntered == true) {
     //cout << "start manipulating nowwww" << endl;
     while (input != "q") {
         hashTable = listToHashArray(*resizedArray, *linkedList, hashTable, drc, hasher);
         cout << "Enter (o)utput, (s)ort, (f)ind, (m)erge, (p)urge, (r)ecords, (h)ash table, or (q)uit: ";
         getline(cin, input);
         if (input == "o") {
             outputDataManipulation(validRecords, resizedArray, lineCount, validRecordsInMem);
         }
         if (input == "s") {
             sortingDataManipulation(*resizedArray, sortedCol);
         }
         if (input == "f") {
             findDataManipulation(resizedArray, sortedCol);
         }
         if (input == "m") {
             mergeDataManipulation(*resizedArray, lineCount, date, validRecords, timestamps, validRecordsInMem, *linkedList, hashTable, drc, hasher);
         }
         if (input == "p") {
             purgeDataManipulation(*linkedList, lineCount, date, validRecords, timestamps, validRecordsInMem, *resizedArray, hashTable, drc, hasher);
         }
         if (input == "r") {
             recordsDataManipulation(linkedList, lineCount, validRecords, validRecordsInMem);
         }
         if (input == "h") {
             hashTableDataManipulation(*drc, hashTable, lineCount, validRecords, validRecordsInMem);
         }
         if (input == "q") {
             cout << "Thanks for using Driller." << endl;
         }
     }
     }
     else {
     return 0;
     }
    return 0;
}
