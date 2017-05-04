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
 * Function to evaluate in expression level in RD parser algorithm
 */
Expression* Parser::expression() {
    Token lastToken = myCurrentToken;
    Expression* retVal = term();
    
    while (myCurrentToken == TOK_PLUS || myCurrentToken == TOK_MINUS) {
        lastToken = myCurrentToken;
        myCurrentToken = getToken();
        Expression* exp = expression();
        
        retVal = new BinaryExpression(retVal, exp, lastToken == TOK_PLUS ? OP_PLUS : OP_MINUS);
    }
    
    return retVal;
}


/*
 * Function to evaluate in term level in RD parser algorithm
 */
Expression* Parser::term() {
    Token lastToken = myCurrentToken;
    Expression* retVal = factor();
    
    while (myCurrentToken == TOK_MUL || myCurrentToken == TOK_DIV) {
        lastToken = myCurrentToken;
        myCurrentToken = getToken();
        Expression* exp = term();
        
        retVal = new BinaryExpression(retVal, exp, lastToken == TOK_MUL ? OP_MUL : OP_DIV);
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
            
            if (myCurrentToken != TOK_CLOS_PAREN) {
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
            
            retVal = new UnaryExpression(retVal, lastToken == TOK_PLUS ? OP_PLUS : OP_MINUS);
            break;
        }
        
        default:
            exit_with_message("Error while analysing tokens");
    }
    
    return retVal;
}

