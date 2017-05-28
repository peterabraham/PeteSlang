//
//  Builder.cpp
//  PeteSlang
//
//  Created by Peter on 03/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#include "Builder.hpp"


/*
 * Function to return Expression from the Expression builder.
 */
Expression* ExpressionBuilder::getExpression() {
    
    Parser* parser = NULL;
    
    try {
        parser = new Parser(myExprString);
    }
    catch(...) {
        exit_with_message("Unhandled excpetion occured");
    }
    
    return nullptr;//parser->callExpression();
}
