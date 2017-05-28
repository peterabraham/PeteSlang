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
#include <string>
#include <sstream>

/*
 * Type to represent Operators used in PeteSlang
 */
enum Operator {
    OP_ILLEGAL = -1,
    OP_PLUS = 1,
    OP_MINUS,
    OP_MUL,
    OP_DIV
};

// Token count to populate ValueTable
const int KEYWORD_COUNT = 7;

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
    TOK_NULL,       // End of string
    
    // Step 3 inclusion
    TOK_PRINT,      // Print Statement
    TOK_PRINTLN,    // PrintLine
    TOK_UNQUOTED_STRING,
    TOK_SEMI,       // ; Semi column
    
    // Step 4 inclusion
    TOK_VAR_NUMBER, // NUMBER data type
    TOK_VAR_STRING, // STRING data type
    TOK_VAR_BOOL,   // Bool data type
    TOK_NUMERIC,    // [0-9]+
    TOK_COMMENT,    // Comment token
    TOK_BOOL_TRUE,  // Boolean TRUE
    TOK_BOOL_FALSE, // Boolean FALSE
    TOK_STRING,     // String Literal
    TOK_ASSIGN      // Assignment Symbol =
};


/*
 * Type info enumerations
 */
enum TypeInfo {
    TYPE_ILLEGAL = -1, // NOT A TYPE
    TYPE_NUMERIC = 1,      // IEEE Double precision floating point
    TYPE_BOOL,         // Boolean Data type
    TYPE_STRING,       // String data type
};

/*
 * Symbol Table entry for variable using Attributes
 */
class SymbolInfo {
public:
    std::string mySymbolName;
    TypeInfo    myType;
    std::string myStrVal;
    double      myDblVal;
    bool        myBoolVal;
};


//
// Common function declarations
//
void exit_with_message(const char *err_msg_i);


/*
 * Function to safe delete a pointer
 */
template<class T> void safe_delete(T*& pVal_i) {
    delete pVal_i;
    pVal_i = NULL;
}


/*
 * Function to safe delete an array
 */
template<class T> void safe_delete_array(T*& pVal_i) {
    delete[] pVal_i;
    pVal_i = NULL;
}


/*
 * Function to safe delete an array of pointers
 */
template<class T> void safe_delete_pointer_array(T*& pVal_i, int count_i) {
    for (int idx = 0; idx < count_i; idx++) {
        delete pVal_i[idx];
        pVal_i[idx] = NULL;
    }
    safe_delete_array(pVal_i);
}

#endif /* Common_hpp */
