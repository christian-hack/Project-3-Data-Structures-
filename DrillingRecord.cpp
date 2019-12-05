//
//  DrillingRecord.cpp
//  Project 3
//
//  Created by Christian Hackelman on 11/7/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

#include "DrillingRecord.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

ostream& operator<<(std::ostream& os, const DrillingRecord& record) {
    os << record.getString(0) << ";" << record.getString(1) << ";"; //prints out the date and timestamp
    for (int i = 0; i < 15; ++i) {
        os << fixed << setprecision(2) << record.getNum(i) << ";";
    }                                                               //iterates through record to print out fixed float values
    os << fixed << setprecision(2) << record.getNum(15);    //print last fixed float value with endline and without semicolon
    return os;
}
DrillingRecord::DrillingRecord() {
    for (int i = 0; i < 16; ++i) {
        nums[i] = 0.0;
    }
    strings[0] = " ";
    strings[1] = " ";
}
void DrillingRecord::addNum(double num) {
    nums[numCtr] = num;
    ++numCtr;
}

void DrillingRecord::addString(string string) {
    strings[strCtr] = string;
    ++strCtr;
}

double DrillingRecord::getNum(unsigned int index) const {
    return nums[index];
}

string DrillingRecord::getString(unsigned int index) const {
    return strings[index];
}

void DrillingRecord::setNum(double num, unsigned int index) {
    nums[index] = num;
}

void DrillingRecord::setString(string string, unsigned int index) {
    strings[index] = string;
}
