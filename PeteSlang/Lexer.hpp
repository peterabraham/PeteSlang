//
//  Lexer.hpp
//  PeteSlang
//
//  Created by Peter on 03/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#ifndef Lexer_hpp
#define Lexer_hpp

#include <iostream>
#include <string>
#include "AST.hpp"

using namespace std;

/*
 * A naive Lexical analyzer which looks for operators , Parenthesis
 * and number. All numbers are treated as IEEE doubles. Only numbers
 * without decimals can be entered.
 */
class Lexer {
    
public:
    Lexer(const string& expr_i);
    Token getToken();
    double getNumber() {return myNumber;}
    
private:
    Lexer(Lexer& lex_i) {}
    Lexer& operator=(const Lexer& lex_i) {return *this;}
    
private:
    string myExpr;
    int    myIndex;
    int    myExpLength;
    double myNumber;
};

#endif /* Lexer_hpp */
