//
//  Lexer.cpp
//  PeteSlang
//
//  Created by Peter on 03/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#include "Frontend/Lexer.h"
#include <algorithm>
#include <cctype>


/*
 * Ctor of the class Lexer
 */
Lexer::Lexer(const string& expr_i) : myExpr(expr_i),
                                     myExpLength(static_cast<int>(expr_i.length())),
                                     myIndex(0)
{
    pmyValTable    = new ValueTable*[KEYWORD_COUNT];

    pmyValTable[0]  = new ValueTable(Token::TOK_BOOL_FALSE, "FALSE");
    pmyValTable[1]  = new ValueTable(Token::TOK_BOOL_TRUE, "TRUE");
    pmyValTable[2]  = new ValueTable(Token::TOK_VAR_STRING, "STRING");
    pmyValTable[3]  = new ValueTable(Token::TOK_VAR_BOOL, "BOOLEAN");
    pmyValTable[4]  = new ValueTable(Token::TOK_VAR_NUMBER, "NUMERIC");
    pmyValTable[5]  = new ValueTable(Token::TOK_PRINT, "PRINT");
    pmyValTable[6]  = new ValueTable(Token::TOK_PRINTLN, "PRINTLINE");
    pmyValTable[7]  = new ValueTable(Token::TOK_IF, "IF");
    pmyValTable[8]  = new ValueTable(Token::TOK_WHILE, "WHILE");
    pmyValTable[9]  = new ValueTable(Token::TOK_WEND, "WEND");
    pmyValTable[10] = new ValueTable(Token::TOK_ELSE, "ELSE");
    pmyValTable[11] = new ValueTable(Token::TOK_ENDIF, "ENDIF");
    pmyValTable[12] = new ValueTable(Token::TOK_THEN, "THEN");
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
    Token tok = Token::TOK_ILLEGAL;
    char nextChar;

    while ((myIndex < myExpLength) && ((nextChar = myExpr[myIndex] == ' ') || (myExpr[myIndex] == '\t')))
        myIndex++;

    if (myIndex == myExpLength)
        return Token::TOK_NULL;

    switch (myExpr[myIndex]) {
        case '\r':
        case '\n': {
            myIndex++;
            goto start;
        }

        case '+': {
            tok = Token::TOK_PLUS;
            myIndex++;
            break;
        }

        case '-': {
            tok = Token::TOK_MINUS;
            myIndex++;
            break;
        }

        case '*': {
            tok = Token::TOK_MUL;
            myIndex++;
            break;
        }

        case '/': {
            if (myExpr[myIndex + 1] == '/') {
                skipToEOL();
                goto start;
            }else {
                tok = Token::TOK_DIV;
                myIndex++;
            }
            break;
        }

        case '(': {
            tok = Token::TOK_OPEN_PAREN;
            myIndex++;
            break;
        }

        case ')': {
            tok = Token::TOK_CLOS_PAREN;
            myIndex++;
            break;
        }

        case ';': {
            tok = Token::TOK_SEMI;
            myIndex++;
            break;
        }

        case '!': {
            tok = Token::TOK_NOT;
            myIndex++;
            break;
        }

        case '>': {
            if (myExpr[myIndex + 1] == '=') {
                tok = Token::TOK_GTE;
                myIndex += 2;
            } else {
                tok = Token::TOK_GT;
                myIndex++;
            }
            break;
        }

        case '<': {
            if (myExpr[myIndex + 1] == '=') {
                tok = Token::TOK_LTE;
                myIndex += 2;
            } else if (myExpr[myIndex + 1] == '>') {
                tok = Token::TOK_NEQ;
                myIndex += 2;
            } else {
                tok = Token::TOK_LT;
                myIndex++;
            }
            break;
        }

        case '=': {
            if (myExpr[myIndex + 1] == '=') {
                tok = Token::TOK_EQ;
                myIndex += 2;
            } else {
                tok = Token::TOK_ASSIGN;
                myIndex++;
            }
            break;
        }

        case '&': {
            if (myExpr[myIndex + 1] == '&') {
                tok = Token::TOK_AND;
                myIndex += 2;
            } else {
                tok = Token::TOK_ILLEGAL;
                myIndex++;
            }
            break;
        }

        case '|': {
            if (myExpr[myIndex + 1] == '|') {
                tok = Token::TOK_OR;
                myIndex += 2;
            } else {
                tok = Token::TOK_ILLEGAL;
                myIndex++;
            }
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
                tok = Token::TOK_ILLEGAL;
            }else {
                myIndex++;
                myLastString = str;
                tok = Token::TOK_STRING;
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
                tok = Token::TOK_NUMERIC;
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
                tok =  Token::TOK_UNQUOTED_STRING;
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
