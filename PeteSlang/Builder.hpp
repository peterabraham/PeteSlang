//
//  Builder.hpp
//  PeteSlang
//
//  Created by Peter on 03/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#ifndef Builder_hpp
#define Builder_hpp

#include "Parser.hpp"

/*
 * Base class for all the Builders
 */
class AbstractBuilder {
};


/*
 * Expression builder class
 */
class ExpressionBuilder : public AbstractBuilder {

public:
    ExpressionBuilder(const string exprStr_i) : myExprString(exprStr_i) {}
    Expression* getExpression();
    
private:
    ExpressionBuilder(ExpressionBuilder& expBuilder_i) {}
    ExpressionBuilder& operator=(const ExpressionBuilder& expBuilder_i) {return *this;}
    
private:
    string myExprString;
};


#endif /* Builder_hpp */
