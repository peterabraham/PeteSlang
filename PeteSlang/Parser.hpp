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
 */
class Parser : public Lexer {
    
public:
    Parser(const string expr_i) : Lexer(expr_i) {}
    Expression* callExpression();
    vector<Statement*> parse();
    
protected:
    Token getNext();
    
private:
    Expression* expression();
    Expression* term();
    Expression* factor();
    
    vector<Statement*> statementList();
    Statement* getStatement();
    Statement* parsePrintStatement();
    Statement* parsePrintLineStatement();
    
    Parser(Parser& parser_i) : Lexer("") {}
    Parser& operator=(const Parser& parser_i) {return *this;}
};

#endif /* Parser_hpp */
