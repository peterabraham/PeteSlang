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
    pmyValTable    = new ValueTable*[KEYWORD_COUNT];
    
    pmyValTable[0] = new ValueTable(TOK_BOOL_FALSE, "FALSE");
    pmyValTable[1] = new ValueTable(TOK_BOOL_TRUE, "TRUE");
    pmyValTable[2] = new ValueTable(TOK_VAR_STRING, "STRING");
    pmyValTable[3] = new ValueTable(TOK_VAR_BOOL, "BOOLEAN");
    pmyValTable[4] = new ValueTable(TOK_VAR_NUMBER, "NUMERIC");
    pmyValTable[5] = new ValueTable(TOK_PRINT, "PRINT");
    pmyValTable[6] = new ValueTable(TOK_PRINTLN, "PRINTLINE");
}

/*
 * Dtor of class Lexer
 */
Lexer::~Lexer() {
    safe_delete_pointer_array(pmyValTable, KEYWORD_COUNT);
}



/*
 * Function to grab next token from the stream. 
 */
Token Lexer::getToken() {
start:
    Token tok = TOK_ILLEGAL;
    char nextChar;
    
    while ((myIndex < myExpLength) && ((nextChar = myExpr[myIndex] == ' ') || (myExpr[myIndex] == '\t')))
        myIndex++;
    
    if (myIndex == myExpLength)
        return TOK_NULL;
    
    switch (myExpr[myIndex]) {
        case '\r':
        case '\n': {
            myIndex++;
            goto start;
        }
        
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
            if (myExpr[myIndex + 1] == '/') {
                skipToEOL();
                goto start;
            }else {
                tok = TOK_DIV;
                myIndex++;
            }
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
        
        case ';': {
            tok = TOK_SEMI;
            myIndex++;
            break;
        }
            
        case '=': {
            tok = TOK_ASSIGN;
            myIndex++;
            break;
        }
           
        case '"': {
            string str = "";
            myIndex++;
            while ((myIndex < myExpLength) && myExpr[myIndex] != '"' ) {
                str += myExpr[myIndex];
                myIndex++;
            }
            if (myIndex == myExpLength) {
                tok = TOK_ILLEGAL;
            }else {
                myIndex++;
                myLastString = str;
                tok = TOK_STRING;
            }
            break;
        }
        
        default: {
            
            if (isdigit(myExpr[myIndex])) {
                string str = "";
                while ((myExpr[myIndex] == '0' || myExpr[myIndex] == '1' || myExpr[myIndex] == '2' ||
                        myExpr[myIndex] == '3' || myExpr[myIndex] == '4' || myExpr[myIndex] == '5' ||
                        myExpr[myIndex] == '6' || myExpr[myIndex] == '7' || myExpr[myIndex] == '8' ||
                        myExpr[myIndex] == '9') && (myIndex < myExpLength))
                {
                    str += myExpr[myIndex];
                    myIndex++;
                }
                
                if ('.' == myExpr[myIndex]) {
                    str += '.';
                    myIndex++;
                    while ((myExpr[myIndex] == '0' || myExpr[myIndex] == '1' || myExpr[myIndex] == '2' ||
                            myExpr[myIndex] == '3' || myExpr[myIndex] == '4' || myExpr[myIndex] == '5' ||
                            myExpr[myIndex] == '6' || myExpr[myIndex] == '7' || myExpr[myIndex] == '8' ||
                            myExpr[myIndex] == '9') && (myIndex < myExpLength))
                    {
                        str += myExpr[myIndex];
                        myIndex++;
                    }
                }
                
                myNumber = atof(str.c_str());
                tok = TOK_NUMERIC;
                break;
            }
            
            if (isalpha(myExpr[myIndex])) {
                string str = "";
                string var = "";
                str += myExpr[myIndex];
                
                while ((++myIndex < myExpLength) && (isalnum(myExpr[myIndex]) || '_' == myExpr[myIndex])) {
                    str += myExpr[myIndex];
                }
                var = str;
                
                // Convert to upper case
                transform(str.begin(), str.end(), str.begin(), ::toupper);
                
                for(int idx = 0; idx < KEYWORD_COUNT; idx++) {
                    if(str.compare(pmyValTable[idx]->value) == 0) {
                        ValueTable* t = pmyValTable[idx];
                        return t->token;
                    }
                }
                
                myLastString = var;
                tok =  TOK_UNQUOTED_STRING;
            } else {
                exit_with_message("Error while analysing tokens");
            }
            break;
        }
    }
    
    return tok;
}


/*
 * Skip to the End of Line
 */
void Lexer::skipToEOL() {
    while (myIndex < myExpLength && '\r' != myExpr[myIndex] && '\n' != myExpr[myIndex]) {
        myIndex++;
    }
    
    if (myIndex == myExpLength) {
        return;
    }
    
    if ( '\n' == myExpr[myIndex + 1]) {
        myIndex += 2;
        return;
    }
    
    myIndex++;
    return;
}
