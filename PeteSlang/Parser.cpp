//
//  Parser.cpp
//  PeteSlang
//
//  Created by Peter on 03/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#include "Parser.hpp"


/*
 * Entry point to evaluate an expression
 */
Expression* Parser::callExpression() {
    myCurrentToken = getToken();
    return expression();
}


/*
 * The new Parser entry point
 */
vector<Statement*> Parser::parse() {
    getNext();
    
    //  Return all parsed statements
    return statementList();
}


/*
 * This function persists the last token,  
 * before we move on to the next token.
 */
Token Parser::getNext() {
    myLastToken = myCurrentToken;
    myCurrentToken = getToken();
    return myCurrentToken;
}


/*
 * Function to evaluate in expression level in RD parser algorithm
 */
Expression* Parser::expression() {
    Token lastToken = myCurrentToken;
    Expression* retVal = term();
    
    while (TOK_PLUS == myCurrentToken || TOK_MINUS == myCurrentToken) {
        lastToken = myCurrentToken;
        myCurrentToken = getToken();
        Expression* exp = expression();
        
        retVal = new BinaryExpression(retVal, exp, TOK_PLUS == lastToken ? OP_PLUS : OP_MINUS);
    }
    
    return retVal;
}


/*
 * Function to evaluate in term level in RD parser algorithm
 */
Expression* Parser::term() {
    Token lastToken = myCurrentToken;
    Expression* retVal = factor();
    
    while (TOK_MUL == myCurrentToken|| TOK_DIV == myCurrentToken) {
        lastToken = myCurrentToken;
        myCurrentToken = getToken();
        Expression* exp = term();
        
        retVal = new BinaryExpression(retVal, exp, TOK_MUL == lastToken ? OP_MUL : OP_DIV);
    }
    
    return retVal;
}


/*
 * Function to evaluate in factor level in RD parser algorithm
 */
Expression* Parser::factor() {
    Token lastToken = myCurrentToken;
    Expression* retVal = NULL;
    
    switch (myCurrentToken) {
        case TOK_DOUBLE: {
            retVal = new NumericConstant(getNumber());
            myCurrentToken = getToken();
            break;
        }
        
        case TOK_OPEN_PAREN: {
            myCurrentToken = getToken();
            retVal = expression();
            
            if (TOK_CLOS_PAREN != myCurrentToken) {
                exit_with_message("Missing Closing Parenthesis");
            }
            myCurrentToken = getToken();
            break;
        }
        
        case TOK_PLUS:
        case TOK_MINUS:
        {
            lastToken = myCurrentToken;
            myCurrentToken = getToken();
            retVal = factor();
            
            retVal = new UnaryExpression(retVal, TOK_PLUS == lastToken ? OP_PLUS : OP_MINUS);
            break;
        }
        
        default:
            exit_with_message("Error while analysing tokens");
    }
    
    return retVal;
}


/*
 * Parse and return the statements.
 */
vector<Statement*> Parser::statementList() {
    vector<Statement*> statements;
    
    while (myCurrentToken != TOK_NULL) {
        Statement* st = getStatement();
        if (NULL != st) {
            statements.insert(statements.end(), st);
        }
    }
    return statements;
}


/*
 * This Routine Queries Statement Type
 * to take the appropriate Branch
 */
Statement* Parser::getStatement() {
    Statement* retval = nullptr;
    switch (myCurrentToken) {
        case TOK_PRINT: {
            retval = parsePrintStatement();
            getNext();
            break;
        }
        case TOK_PRINTLN: {
            retval = parsePrintLineStatement();
            getNext();
            break;
        }
        default:
        exit_with_message("Error while analysing tokens");
    }
    return retval;
}


/*
 * Function to parse the print statement. 
 * Grammer: PRINT <expr>
 * The function is expecting a valid expression(which will be compiled)
 * and a semi collon to terminate the line.
 * Create a PrintStatement object on successful evaluation.
 */
Statement* Parser::parsePrintStatement() {
    getNext();
    Expression* exp = expression();
    if(TOK_SEMI != myCurrentToken) {
        exit_with_message("\n; is expected");
    }
    
    return new PrintStatement(exp);
}


/*
 * Function to parse the print line statement.
 * Grammer: PRINTLINE <expr>
 * The function is expecting a valid expression(which will be compiled)
 * and a semi collon to terminate the line.
 * Create a PrintLineStatement object on successful evaluation.
 */
Statement* Parser::parsePrintLineStatement() {
    getNext();
    Expression* exp = expression();
    if(TOK_SEMI != myCurrentToken) {
        exit_with_message(": is expected");
    }
    
    return new PrintLineStatement(exp);
}
