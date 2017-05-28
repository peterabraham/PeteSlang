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
Expression* Parser::callExpression(CompilationContext* context_i) {
    myCurrentToken = getToken();
    return expression(context_i);
}


/*
 * The new Parser entry point
 */
vector<Statement*> Parser::parse(CompilationContext* context_i) {
    getNext();
    
    //  Return all parsed statements
    return statementList(context_i);
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
 * Parse and return the statements.
 */
vector<Statement*> Parser::statementList(CompilationContext* context_i) {
    vector<Statement*> statements;
    
    while (myCurrentToken != TOK_NULL) {
        Statement* st = getStatement(context_i);
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
Statement* Parser::getStatement(CompilationContext* context_i) {
    Statement* retval = nullptr;
    switch (myCurrentToken) {
        case TOK_VAR_STRING:
        case TOK_VAR_NUMBER:
        case TOK_VAR_BOOL: {
            retval = parseVariableDeclStatement(context_i);
            getNext();
            break;
        }
        case TOK_PRINT: {
            retval = parsePrintStatement(context_i);
            getNext();
            break;
        }
        case TOK_PRINTLN: {
            retval = parsePrintLineStatement(context_i);
            getNext();
            break;
        }
        case TOK_UNQUOTED_STRING: {
            retval = parseAssignmentStatement(context_i);
            getNext();
            break;
        }
        default:
            exit_with_message("Error while analysing tokens");
    }
    return retval;
}


/*
 * Function to evaluate in expression level in RD parser algorithm
 */
Expression* Parser::expression(CompilationContext* context_i) {
    Token lastToken = myCurrentToken;
    Expression* retVal = term(context_i);
    
    while (TOK_PLUS == myCurrentToken || TOK_MINUS == myCurrentToken) {
        lastToken = myCurrentToken;
        myCurrentToken = getToken();
        Expression* exp = expression(context_i);
        
        if (TOK_PLUS == lastToken) {
            retVal = new BinaryPlus(retVal, exp);
        } else {
            retVal = new BinaryMinus(retVal, exp);
        }
    }
    
    return retVal;
}


/*
 * Function to evaluate in term level in RD parser algorithm
 */
Expression* Parser::term(CompilationContext* context_i) {
    Token lastToken = myCurrentToken;
    Expression* retVal = factor(context_i);
    
    while (TOK_MUL == myCurrentToken|| TOK_DIV == myCurrentToken) {
        lastToken = myCurrentToken;
        myCurrentToken = getToken();
        Expression* exp = term(context_i);
        
        if (TOK_MUL == lastToken) {
            retVal = new Multiply(retVal, exp);
        } else {
            retVal = new Divide(retVal, exp);
        }
    }
    
    return retVal;
}


/*
 * Function to evaluate in factor level in RD parser algorithm
 */
Expression* Parser::factor(CompilationContext* context_i) {
    Token lastToken = myCurrentToken;
    Expression* retVal = NULL;
    
    switch (myCurrentToken) {
        case TOK_NUMERIC: {
            retVal = new NumericConstant(getNumber());
            myCurrentToken = getToken();
            break;
        }
        case TOK_STRING: {
            retVal = new StringLiteral(myLastString);
            myCurrentToken = getToken();
            break;
        }
        case TOK_BOOL_TRUE:
        case TOK_BOOL_FALSE:{
            retVal = new BooleanConstant(myCurrentToken == TOK_BOOL_TRUE ? true : false);
            myCurrentToken = getToken();
            break;
        }
        case TOK_OPEN_PAREN: {
            myCurrentToken = getToken();
            retVal = expression(context_i);
            
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
            retVal = factor(context_i);
            
            if( TOK_PLUS == lastToken) {
                retVal = new UnaryPlus(retVal);
            } else {
                retVal = new UnaryMinus(retVal);
            }
            break;
        }
        case TOK_UNQUOTED_STRING: {
            SymbolInfo* symbol = context_i->getInfo(myLastString);
            if (nullptr == symbol) {
                exit_with_message("Undefined symbol");
            }
            
            getNext();
            retVal = new Variable(symbol);
            break;
        }
        
        default:
            exit_with_message("illegal token");
    }
    
    return retVal;
}


/*
 * Function to parse the print statement. 
 * Grammer: PRINT <expr>
 * The function is expecting a valid expression(which will be compiled)
 * and a semi collon to terminate the line.
 * Create a PrintStatement object on successful evaluation.
 */
Statement* Parser::parsePrintStatement(CompilationContext* context_i) {
    getNext();
    Expression* exp = expression(context_i);
    if (TOK_SEMI != myCurrentToken) {
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
Statement* Parser::parsePrintLineStatement(CompilationContext* context_i) {
    getNext();
    Expression* exp = expression(context_i);
    if (TOK_SEMI != myCurrentToken) {
        exit_with_message(": is expected");
    }
    
    return new PrintLineStatement(exp);
}


/*
 * Function to parse Variable declaration statement
 */
Statement* Parser::parseVariableDeclStatement(CompilationContext* context_i) {
    Token tok = myCurrentToken;
    getNext();
    
    if (TOK_UNQUOTED_STRING == myCurrentToken) {
        SymbolInfo* symbol = new SymbolInfo();
        symbol->mySymbolName = myLastString;
        symbol->myType = (TOK_VAR_BOOL == tok) ?
            TYPE_BOOL : (TOK_VAR_NUMBER == tok) ?
            TYPE_NUMERIC : TYPE_STRING;
        
        getNext();
        
        if (TOK_SEMI == myCurrentToken) {
            context_i->addInfo(symbol);
            return new VariableDeclStatement(symbol);
        }else {
            exit_with_message(", or ; expected");
        }
    }else {
        exit_with_message("Invalid variable declaration");
    }
    return nullptr;
}


/*
 * Function to parse the assignment statement
 * Retrieve the variable and look it up in
 * the symbol table, if not found throw exception
 */
Statement* Parser::parseAssignmentStatement(CompilationContext* context_i) {
    string var = myLastString;
    SymbolInfo* symbol = context_i->getInfo(var);
    if (nullptr == symbol) {
        exit_with_message("Variable not found");
    }
    
    getNext();
    
    if (TOK_ASSIGN != myCurrentToken) {
        exit_with_message("= expected");
    }
    
    getNext();
    
    // Skip the token to start the expression
    // parsing on the RHS
    Expression* exp = expression(context_i);
    
    // Do the type analysis
//    if (exp->typeCheck(context_i) != symbol->myType) {
//        exit_with_message("Type mismatch in assignment");
//    }
    
    if (TOK_SEMI != myCurrentToken) {
        exit_with_message("; expected");
    }
    
    return new AssignmentStatement(symbol, exp);
}
