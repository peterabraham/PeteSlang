//
//  Common.h
//  PeteSlang
//
//  Common functions and enums used in PeteSlang.
//
//  Created by Peter on 29/04/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <variant>

/*
 * Type to represent Operators used in PeteSlang
 */
enum class Operator {
    OP_ILLEGAL = -1,
    OP_PLUS = 1,
    OP_MINUS,
    OP_MUL,
    OP_DIV
};

/*
 * Type to represent Relational
 * Operators used in PeteSlang
 */
enum class RelationalOperator {
    REL_OP_ILLEGAL = -1,
    REL_OP_EQ = 1, // '=='
    REL_OP_NEQ,    // '<>'
    REL_OP_GT,     // '>'
    REL_OP_GTE,    // '>='
    REL_OP_LT,     // '<'
    REL_OP_LTE     // '<='
};

// Token count to populate ValueTable
inline constexpr int KEYWORD_COUNT = 13;

/*
 * Tokens in PeteSlang
 */
enum class Token {
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
    TOK_ASSIGN,     // Assignment Symbol =

    // Added in step 6
    // For relational & logical operator support
    TOK_EQ,         // '=='
    TOK_NEQ,        // '<>'
    TOK_GT,         // '>'
    TOK_GTE,        // '>='
    TOK_LT,         // '<'
    TOK_LTE,        // '<='
    TOK_AND,        // '&&'
    TOK_OR,         // '||'
    TOK_NOT,        // '!'

    // Control structures support
    TOK_IF,         // IF
    TOK_THEN,       // Then
    TOK_ELSE,       // Else Statement
    TOK_ENDIF,      // Endif Statement
    TOK_WHILE,      // WHILE
    TOK_WEND        // Wend Statement
};


/*
 * Type info enumerations
 */
enum class TypeInfo {
    TYPE_ILLEGAL = -1, // NOT A TYPE
    TYPE_NUMERIC = 1,  // IEEE Double precision floating point
    TYPE_BOOL,         // Boolean Data type
    TYPE_STRING,       // String data type
};

/*
 * Symbol Table entry for variable using Attributes.
 *
 * The value used to be three simultaneously-live fields
 * (myStrVal / myDblVal / myBoolVal), which made it possible to read
 * the wrong one for myType. It is now a std::variant, so only the
 * alternative matching myType can ever be alive. getDouble()/getBool()/
 * getString() throw std::bad_variant_access if you read the wrong
 * alternative instead of silently returning garbage.
 */
class SymbolInfo {
public:
    std::string mySymbolName;
    TypeInfo    myType = TypeInfo::TYPE_ILLEGAL;
    std::variant<double, bool, std::string> myValue;

    double getDouble() const { return std::get<double>(myValue); }
    bool getBool() const { return std::get<bool>(myValue); }
    const std::string& getString() const { return std::get<std::string>(myValue); }

    void setDouble(double v) { myValue = v; }
    void setBool(bool v) { myValue = v; }
    void setString(std::string v) { myValue = std::move(v); }
};


//
// Common function declarations
//
[[noreturn]] void exit_with_message(const char* err_msg_i);


/*
 * Function to safe delete a pointer
 */
template<class T> void safe_delete(T*& pVal_i) {
    delete pVal_i;
    pVal_i = nullptr;
}


/*
 * Function to safe delete an array
 */
template<class T> void safe_delete_array(T*& pVal_i) {
    delete[] pVal_i;
    pVal_i = nullptr;
}


/*
 * Function to safe delete an array of pointers
 */
template<class T> void safe_delete_pointer_array(T*& pVal_i, int count_i) {
    for (int idx = 0; idx < count_i; idx++) {
        delete pVal_i[idx];
        pVal_i[idx] = nullptr;
    }
    safe_delete_array(pVal_i);
}
