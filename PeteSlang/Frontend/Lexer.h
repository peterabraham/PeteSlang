//
//  Lexer.h
//  PeteSlang
//
//  Created by Peter on 03/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#pragma once

#include "AST/Expressions.h"
#include "AST/Statements.h"

using namespace std;

/*
 * Keyword Table Entry
 */
struct ValueTable {
    Token token;
    string value;

    ValueTable() {}

    ValueTable(Token token_i, string value_i) {
        token = token_i;
        value = value_i;
    }
};


/*
 * A naive Lexical analyzer which looks for operators , Parenthesis
 * and number. All numbers are treated as IEEE doubles. Only numbers
 * without decimals can be entered.
 */
class Lexer {

public:
    Lexer(const string& expr_i);
    virtual ~Lexer();
    Token getToken();
    double getNumber() {return myNumber;}

    Lexer(const Lexer&) = delete;
    Lexer& operator=(const Lexer&) = delete;

private:
    void skipToEOL();

protected:
    Token  myCurrentToken; // Current Token
    Token  myLastToken;    // Penultimate token
    string myLastString;   // Last unquoted string

private:
    string      myExpr;
    int         myIndex;
    int         myExpLength;
    double      myNumber;
    ValueTable** pmyValTable;
};
