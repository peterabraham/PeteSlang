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

/*
 * Parser is derived from the Lexer class. This class evaluate the
 * expressions by using an algorithm by the name Recursive descent parsing.
 */
class Parser : public Lexer {
    
public:
    Parser(const string expr_i) : Lexer(expr_i) {}
    Expression* callExpression();
    
private:
    Expression* expression();
    Expression* term();
    Expression* factor();
    Parser(Parser& parser_i) : Lexer("") {}
    Parser& operator=(const Parser& parser_i) {return *this;}
    
private:
    Token myCurrentToken;
};

#endif /* Parser_hpp */