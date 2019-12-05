//
//  Exceptions.hpp
//  Project 3
//
//  Created by Christian Hackelman on 11/7/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

// Do not add or modify anything in this file.

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

class Exception {};
class ExceptionIndexOutOfRange : Exception {};
class ExceptionMemoryNotAvailable : Exception {};
class ExceptionLinkedListAccess : Exception {};
class ExceptionEnumerationBeyondEnd : Exception {};
class ExceptionHashTableAccess : Exception {};

#endif // !EXCEPTIONS_H
