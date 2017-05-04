//
//  Lexer.cpp
//  PeteSlang
//
//  Created by Peter on 03/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#include "Lexer.hpp"


/*
 * Ctor of the class Lexer
 */
Lexer::Lexer(const string& expr_i) : myExpr(expr_i),
                                     myExpLength(static_cast<int>(expr_i.length())),
                                     myIndex(0)
{
}


/*
 * Function to grab next token from the stream. 
 */
Token Lexer::getToken() {
    
    Token tok = TOK_ILLEGAL;
    
    while ((myIndex < myExpLength) && ((myExpr[myIndex] == ' ') || (myExpr[myIndex] == '\t')))
        myIndex++;
    
    if (myIndex == myExpLength)
        return TOK_NULL;
    
    switch (myExpr[myIndex]) {
        case '+': {
            tok = TOK_PLUS;
            myIndex++;
            break;
        }
        
        case '-': {
            tok = TOK_MINUS;
            myIndex++;
            break;
        }
        
        case '*': {
            tok = TOK_MUL;
            myIndex++;
            break;
        }
        
        case '/': {
            tok = TOK_DIV;
            myIndex++;
            break;
        }
        
        case '(': {
            tok = TOK_OPEN_PAREN;
            myIndex++;
            break;
        }
        
        case ')': {
            tok = TOK_CLOS_PAREN;
            myIndex++;
            break;
        }
        
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            string str = "";
            while ((myExpr[myIndex] == '0' || myExpr[myIndex] == '1' || myExpr[myIndex] == '2' ||
                    myExpr[myIndex] == '3' || myExpr[myIndex] == '4' || myExpr[myIndex] == '5' ||
                    myExpr[myIndex] == '6' || myExpr[myIndex] == '7' || myExpr[myIndex] == '8' ||
                    myExpr[myIndex] == '9') && (myIndex < myExpLength))
            {
                str += myExpr[myIndex];
                myIndex++;
            }
            
            myNumber = atof(str.c_str());
            tok = TOK_DOUBLE;
            break;
        }
        
        default:
        exit_with_message("Error while analysing tokens");
        break;
    }
    
    return tok;
}
