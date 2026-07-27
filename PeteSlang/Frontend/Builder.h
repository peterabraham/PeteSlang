//
//  Builder.h
//  PeteSlang
//
//  Created by Peter on 03/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#pragma once

#include "Frontend/Parser.h"
#include <memory>

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
    std::unique_ptr<Expression> getExpression();

    ExpressionBuilder(const ExpressionBuilder&) = delete;
    ExpressionBuilder& operator=(const ExpressionBuilder&) = delete;

private:
    string myExprString;
};
