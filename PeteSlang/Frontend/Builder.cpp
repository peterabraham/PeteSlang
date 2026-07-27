//
//  Builder.cpp
//  PeteSlang
//
//  Created by Peter on 03/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#include "Frontend/Builder.h"
#include <memory>

/*
 * Function to return Expression from the Expression builder.
 */
std::unique_ptr<Expression> ExpressionBuilder::getExpression() {

    std::unique_ptr<Parser> parser;

    try {
        parser = std::make_unique<Parser>(myExprString);
    }
    catch(...) {
        exit_with_message("Unhandled excpetion occured");
    }

    return nullptr;//parser->callExpression();
}
