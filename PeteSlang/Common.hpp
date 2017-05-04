//
//  Common.hpp
//  PeteSlang
//
//  Common functions and enums used in PeteSlang.
//
//  Created by Peter on 29/04/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#ifndef Common_hpp
#define Common_hpp

#include <iostream>

/*
 * Type to represent Operators used in PeteSlang
 */
enum Operator {
    OP_ILLEGAL = -1,
    OP_PLUS,
    OP_MINUS,
    OP_MUL,
    OP_DIV
};

/*
 * Tokens in PeteSlang
 */
enum Token {
    TOK_ILLEGAL = -1,
    TOK_PLUS = 1,   // '+'
    TOK_MINUS,      // '-'
    TOK_MUL,        // '*'
    TOK_DIV,        // '/'
    TOK_OPEN_PAREN, // '('
    TOK_CLOS_PAREN, // ')'
    TOK_DOUBLE,     // '('
    TOK_NULL        // End of string
};


//
// Common function declarations
//
void exit_with_message(const char *err_msg);


/*
 * Function to safe delete a pointer
 */
template<class T> void SafeDelete(T*& pVal) {
    delete pVal;
    pVal = NULL;
}


/*
 * Function to safe delete an array
 */
template<class T> void SafeDeleteArray( T*& pVal ) {
    delete[] pVal;
    pVal = NULL;
}

#endif /* Common_hpp */
