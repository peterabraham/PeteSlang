//
//  Parser.h
//  PeteSlang
//
//  Created by Peter on 03/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#pragma once

#include "Frontend/Lexer.h"
#include <memory>

/*
 * Parser is derived from the Lexer class. This class evaluate the
 * expressions by using an algorithm by the name Recursive descent parsing.
 *
 */
class Parser : public Lexer {

public:
    Parser(const string expr_i) : Lexer(expr_i) {}
    std::unique_ptr<Expression> callExpression(CompilationContext* context_i);
    vector<std::unique_ptr<Statement>> parse(CompilationContext* context_i);

    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

protected:
    Token getNext();

private:
    vector<std::unique_ptr<Statement>> statementList(CompilationContext* context_i);
    std::unique_ptr<Statement> getStatement(CompilationContext* context_i);

    std::unique_ptr<Expression> bExpression(CompilationContext* context_i);
    std::unique_ptr<Expression> lExpression(CompilationContext* context_i);
    RelationalOperator getRelationalOperator(Token tok_i);
    std::unique_ptr<Expression> expression(CompilationContext* context_i);
    std::unique_ptr<Expression> term(CompilationContext* context_i);
    std::unique_ptr<Expression> factor(CompilationContext* context_i);

    std::unique_ptr<Statement> parsePrintStatement(CompilationContext* context_i);
    std::unique_ptr<Statement> parsePrintLineStatement(CompilationContext* context_i);
    std::unique_ptr<Statement> parseVariableDeclStatement(CompilationContext* context_i);
    std::unique_ptr<Statement> parseAssignmentStatement(CompilationContext* context_i);
    std::unique_ptr<Statement> parseIfStatement(CompilationContext* context_i);
    std::unique_ptr<Statement> parseWhileStatement(CompilationContext* context_i);
};
