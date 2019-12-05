//
//  DrillingRecordComparator.cpp
//  Project 3
//
//  Created by Christian Hackelman on 11/7/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

#include "DrillingRecordComparator.h"
#include <iostream>
using namespace std;

DrillingRecordComparator::DrillingRecordComparator(unsigned int column) {
    //DrillingRecordComparator::column = column;
    this->column = column;
}
DrillingRecordComparator::DrillingRecordComparator(DrillingRecordComparator& drillingRecordComparator) {
    this->column = drillingRecordComparator.column;
}
int DrillingRecordComparator::compare(const DrillingRecord &item1, const DrillingRecord &item2)const {
    if (column == 0 || column == 1) {
        string time1 = item1.getString(column);
        string time2 = item2.getString(column);
        if (time1 < time2) {
            return -1;
        }
        else if (time1 > time2) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else if (column >= 2) {
        if (item1.getNum(column-2) < item2.getNum(column-2)) {
            return -1;
        }
        else if (item1.getNum(column-2) > item2.getNum(column-2)) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}
