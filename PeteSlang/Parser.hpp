//
//  Parser.hpp
//  PeteSlang
//
//  Created by Peter on 03/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#ifndef Parser_hpp
#define Parser_hpp

#include "Lexer.hpp"
#include <vector>

/*
 * Parser is derived from the Lexer class. This class evaluate the
 * expressions by using an algorithm by the name Recursive descent parsing.
 *
 */
class Parser : public Lexer {
    
public:
    Parser(const string expr_i) : Lexer(expr_i) {}
    Expression* callExpression(CompilationContext* context_i);
    vector<Statement*> parse(CompilationContext* context_i);
    
protected:
    Token getNext();
    
private:
    vector<Statement*> statementList(CompilationContext* context_i);
    Statement* getStatement(CompilationContext* context_i);
    
    Expression* expression(CompilationContext* context_i);
    Expression* term(CompilationContext* context_i);
    Expression* factor(CompilationContext* context_i);
    
    Statement* parsePrintStatement(CompilationContext* context_i);
    Statement* parsePrintLineStatement(CompilationContext* context_i);
    Statement* parseVariableDeclStatement(CompilationContext* context_i);
    Statement* parseAssignmentStatement(CompilationContext* context_i);
    
    Parser(Parser& parser_i) : Lexer("") {}
    Parser& operator=(const Parser& parser_i) {return *this;}
};

#endif /* Parser_hpp */
