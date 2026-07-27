//
//  Parser.cpp
//  PeteSlang
//
//  Created by Peter on 03/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#include "Frontend/Parser.h"
#include <utility>

/*
 * Entry point to evaluate an expression
 */
std::unique_ptr<Expression> Parser::callExpression(CompilationContext* context_i) {
    myCurrentToken = getToken();
    return expression(context_i);
}


/*
 * The new Parser entry point
 */
vector<std::unique_ptr<Statement>> Parser::parse(CompilationContext* context_i) {
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
vector<std::unique_ptr<Statement>> Parser::statementList(CompilationContext* context_i) {
    vector<std::unique_ptr<Statement>> statements;

    while (Token::TOK_ELSE  != myCurrentToken &&
           Token::TOK_ENDIF != myCurrentToken &&
           Token::TOK_WEND  != myCurrentToken &&
           Token::TOK_NULL  != myCurrentToken) {
        std::unique_ptr<Statement> st = getStatement(context_i);
        if (nullptr != st) {
            statements.push_back(std::move(st));
        }
    }
    return statements;
}


/*
 * This Routine Queries Statement Type
 * to take the appropriate Branch
 */
std::unique_ptr<Statement> Parser::getStatement(CompilationContext* context_i) {
    std::unique_ptr<Statement> retval = nullptr;
    switch (myCurrentToken) {
        case Token::TOK_VAR_STRING:
        case Token::TOK_VAR_NUMBER:
        case Token::TOK_VAR_BOOL: {
            retval = parseVariableDeclStatement(context_i);
            getNext();
            break;
        }
        case Token::TOK_PRINT: {
            retval = parsePrintStatement(context_i);
            getNext();
            break;
        }
        case Token::TOK_PRINTLN: {
            retval = parsePrintLineStatement(context_i);
            getNext();
            break;
        }
        case Token::TOK_IF: {
            retval = parseIfStatement(context_i);
            getNext();
            break;
        }
        case Token::TOK_WHILE: {
            retval = parseWhileStatement(context_i);
            getNext();
            break;
        }
        case Token::TOK_UNQUOTED_STRING: {
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
 * Function to evaluate in logical expression level in RD parser algorithm
 */
std::unique_ptr<Expression> Parser::bExpression(CompilationContext* context_i) {
    Token lastToken = Token::TOK_ILLEGAL;
    std::unique_ptr<Expression> retExp = lExpression(context_i);

    while (Token::TOK_AND == myCurrentToken || Token::TOK_OR == myCurrentToken) {
        lastToken = myLastToken;
        myCurrentToken = getNext();
        std::unique_ptr<Expression> exp = lExpression(context_i);

        retExp = std::make_unique<LogicalExpression>(std::move(retExp), std::move(exp), lastToken);
    }
    return retExp;
}


/*
 * Function to evaluate in relational expression level in RD parser algorithm
 */
std::unique_ptr<Expression> Parser::lExpression(CompilationContext* context_i) {
    Token lastToken = Token::TOK_ILLEGAL;
    std::unique_ptr<Expression> retExp = expression(context_i);

    while (Token::TOK_EQ == myCurrentToken || Token::TOK_NEQ == myCurrentToken ||
           Token::TOK_GT == myCurrentToken || Token::TOK_GTE == myCurrentToken ||
           Token::TOK_LT == myCurrentToken || Token::TOK_LTE == myCurrentToken) {
        lastToken = myCurrentToken;
        myCurrentToken = getNext();
        std::unique_ptr<Expression> exp = expression(context_i);
        RelationalOperator relOp = getRelationalOperator(lastToken);
        retExp = std::make_unique<RelationalExpression>(std::move(retExp), std::move(exp), relOp);
    }
    return retExp;
}


/*
 * Function to return the relational operator
 * evaluating the current token
 */
RelationalOperator Parser::getRelationalOperator(Token tok_i) {
    RelationalOperator relOp = RelationalOperator::REL_OP_ILLEGAL;

    switch (tok_i) {
        case Token::TOK_EQ:
            relOp = RelationalOperator::REL_OP_EQ;
            break;

        case Token::TOK_NEQ:
            relOp = RelationalOperator::REL_OP_NEQ;
            break;

        case Token::TOK_LT:
            relOp = RelationalOperator::REL_OP_LT;
            break;

        case Token::TOK_LTE:
            relOp = RelationalOperator::REL_OP_LTE;
            break;

        case Token::TOK_GT:
            relOp = RelationalOperator::REL_OP_GT;
            break;

        case Token::TOK_GTE:
            relOp = RelationalOperator::REL_OP_GTE;
            break;

        default:
            relOp = RelationalOperator::REL_OP_ILLEGAL;
            break;
    }
    return relOp;
}


/*
 * Function to evaluate in expression level in RD parser algorithm
 */
std::unique_ptr<Expression> Parser::expression(CompilationContext* context_i) {
    Token lastToken = Token::TOK_ILLEGAL;
    std::unique_ptr<Expression> retVal = term(context_i);

    while (Token::TOK_PLUS == myCurrentToken || Token::TOK_MINUS == myCurrentToken) {
        lastToken = myCurrentToken;
        myCurrentToken = getToken();
        std::unique_ptr<Expression> exp = expression(context_i);

        if (Token::TOK_PLUS == lastToken) {
            retVal = std::make_unique<BinaryPlus>(std::move(retVal), std::move(exp));
        } else {
            retVal = std::make_unique<BinaryMinus>(std::move(retVal), std::move(exp));
        }
    }

    return retVal;
}


/*
 * Function to evaluate in term level in RD parser algorithm
 */
std::unique_ptr<Expression> Parser::term(CompilationContext* context_i) {
    Token lastToken = myCurrentToken;
    std::unique_ptr<Expression> retVal = factor(context_i);

    while (Token::TOK_MUL == myCurrentToken|| Token::TOK_DIV == myCurrentToken) {
        lastToken = myCurrentToken;
        myCurrentToken = getToken();
        std::unique_ptr<Expression> exp = term(context_i);

        if (Token::TOK_MUL == lastToken) {
            retVal = std::make_unique<Multiply>(std::move(retVal), std::move(exp));
        } else {
            retVal = std::make_unique<Divide>(std::move(retVal), std::move(exp));
        }
    }

    return retVal;
}


/*
 * Function to evaluate in factor level in RD parser algorithm
 */
std::unique_ptr<Expression> Parser::factor(CompilationContext* context_i) {
    Token lastToken = myCurrentToken;
    std::unique_ptr<Expression> retVal = nullptr;

    switch (myCurrentToken) {
        case Token::TOK_NUMERIC: {
            retVal = std::make_unique<NumericConstant>(getNumber());
            myCurrentToken = getToken();
            break;
        }
        case Token::TOK_STRING: {
            retVal = std::make_unique<StringLiteral>(myLastString);
            myCurrentToken = getToken();
            break;
        }
        case Token::TOK_BOOL_TRUE:
        case Token::TOK_BOOL_FALSE:{
            retVal = std::make_unique<BooleanConstant>(myCurrentToken == Token::TOK_BOOL_TRUE ? true : false);
            myCurrentToken = getToken();
            break;
        }
        case Token::TOK_OPEN_PAREN: {
            myCurrentToken = getToken();
            retVal = bExpression(context_i);

            if (Token::TOK_CLOS_PAREN != myCurrentToken) {
                exit_with_message("Missing Closing Parenthesis");
            }
            myCurrentToken = getToken();
            break;
        }

        case Token::TOK_PLUS:
        case Token::TOK_MINUS:
        {
            lastToken = myCurrentToken;
            myCurrentToken = getToken();
            retVal = factor(context_i);

            if( Token::TOK_PLUS == lastToken) {
                retVal = std::make_unique<UnaryPlus>(std::move(retVal));
            } else {
                retVal = std::make_unique<UnaryMinus>(std::move(retVal));
            }
            break;
        }
        case Token::TOK_NOT:
        {
            lastToken = myCurrentToken;
            getNext();
            retVal = factor(context_i);

            retVal = std::make_unique<LogicalNot>(std::move(retVal));
            break;
        }
        case Token::TOK_UNQUOTED_STRING: {
            SymbolInfo* symbol = context_i->getInfo(myLastString);
            if (nullptr == symbol) {
                exit_with_message("Undefined symbol");
            }

            getNext();
            retVal = std::make_unique<Variable>(symbol);
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
std::unique_ptr<Statement> Parser::parsePrintStatement(CompilationContext* context_i) {
    getNext();
    std::unique_ptr<Expression> exp = bExpression(context_i);
    if (Token::TOK_SEMI != myCurrentToken) {
        exit_with_message("\n; is expected");
    }

    return std::make_unique<PrintStatement>(std::move(exp));
}


/*
 * Function to parse the print line statement.
 * Grammer: PRINTLINE <expr>
 * The function is expecting a valid expression(which will be compiled)
 * and a semi collon to terminate the line.
 * Create a PrintLineStatement object on successful evaluation.
 */
std::unique_ptr<Statement> Parser::parsePrintLineStatement(CompilationContext* context_i) {
    getNext();
    std::unique_ptr<Expression> exp = bExpression(context_i);
    if (Token::TOK_SEMI != myCurrentToken) {
        exit_with_message(": is expected");
    }

    return std::make_unique<PrintLineStatement>(std::move(exp));
}


/*
 * Function to parse Variable declaration statement
 */
std::unique_ptr<Statement> Parser::parseVariableDeclStatement(CompilationContext* context_i) {
    Token tok = myCurrentToken;
    getNext();

    if (Token::TOK_UNQUOTED_STRING == myCurrentToken) {
        auto symbol = std::make_unique<SymbolInfo>();
        symbol->mySymbolName = myLastString;
        symbol->myType = (Token::TOK_VAR_BOOL == tok) ?
            TypeInfo::TYPE_BOOL : (Token::TOK_VAR_NUMBER == tok) ?
            TypeInfo::TYPE_NUMERIC : TypeInfo::TYPE_STRING;

        getNext();

        if (Token::TOK_SEMI == myCurrentToken) {
            // Register the raw, non-owning pointer with the
            // compile-time symbol table before ownership moves into
            // the statement below (see Runtime/SymbolTable.h).
            context_i->addInfo(symbol.get());
            return std::make_unique<VariableDeclStatement>(std::move(symbol));
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
std::unique_ptr<Statement> Parser::parseAssignmentStatement(CompilationContext* context_i) {
    string var = myLastString;
    SymbolInfo* symbol = context_i->getInfo(var);
    if (nullptr == symbol) {
        exit_with_message("Variable not found");
    }

    getNext();

    if (Token::TOK_ASSIGN != myCurrentToken) {
        exit_with_message("= expected");
    }

    getNext();

    // Skip the token to start the expression
    // parsing on the RHS
    std::unique_ptr<Expression> exp = bExpression(context_i);

    // Do the type analysis
//    if (exp->typeCheck(context_i) != symbol->myType) {
//        exit_with_message("Type mismatch in assignment");
//    }

    if (Token::TOK_SEMI != myCurrentToken) {
        exit_with_message("; expected");
    }

    return std::make_unique<AssignmentStatement>(symbol, std::move(exp));
}


std::unique_ptr<Statement> Parser::parseIfStatement(CompilationContext* context_i) {
    getNext();
    std::unique_ptr<Expression> exp = bExpression(context_i);

    if (TypeInfo::TYPE_BOOL != exp->typeCheck(context_i)) {
        exit_with_message("Expects a boolean expression");
    }

    if (Token::TOK_THEN != myCurrentToken) {
        exit_with_message("Then Expected");
    }

    getNext();
    vector<std::unique_ptr<Statement>> truePart = statementList(context_i);
    vector<std::unique_ptr<Statement>> falsePart;

    if( Token::TOK_ENDIF == myCurrentToken) {
        return std::make_unique<IfStatement>(std::move(exp), std::move(truePart), std::move(falsePart));
    }

    if (Token::TOK_ELSE != myCurrentToken) {
        exit_with_message("ELSE expected");
    }

    getNext();
    falsePart = statementList(context_i);
    if( Token::TOK_ENDIF != myCurrentToken) {
        exit_with_message("END IF EXPECTED");
    }

    return std::make_unique<IfStatement>(std::move(exp), std::move(truePart), std::move(falsePart));
}



std::unique_ptr<Statement> Parser::parseWhileStatement(CompilationContext* context_i) {
    getNext();
    std::unique_ptr<Expression> exp = bExpression(context_i);

    if (TypeInfo::TYPE_BOOL != exp->typeCheck(context_i)) {
        exit_with_message("Expects a boolean expression");
    }

    vector<std::unique_ptr<Statement>> body = statementList(context_i);

    if (Token::TOK_WEND != myCurrentToken) {
        exit_with_message("Wend Expected");
    }

    return std::make_unique<WhileStatement>(std::move(exp), std::move(body));
}
