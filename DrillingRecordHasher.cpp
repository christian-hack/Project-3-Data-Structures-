//
//  DrillingRecordHasher.cpp
//  Project 3
//
//  Created by Christian Hackelman on 11/7/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

#include "DrillingRecordHasher.h"

using namespace std;

unsigned long DrillingRecordHasher::hash(const DrillingRecord &item) const {
    unsigned long sum = 0;
    int hash = 0;
    for (unsigned int i = 0; i < item.getString(1).length(); ++i) {
        hash = int(item.getString(1)[i]);
        sum += hash;
    }
    return sum;
}
