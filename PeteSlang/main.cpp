//
//  main.cpp
//  PeteSlang
//
//  Created by Peter on 29/04/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#include <iostream>
#include "Builder.hpp"

int main(int argc, const char * argv[]) {

    
//    NumericConstant c1(5), c2(10), c3(30), c4(50);
//    
//    Expression* e = new BinaryExpression(&c1, &c2, OP_MUL);
//    std::cout << e->evaluate(NULL) << std::endl;
//    delete e; e = NULL;
//                                             
//    e = new UnaryExpression(new BinaryExpression(&c2, new BinaryExpression(&c3, &c4, OP_PLUS), OP_MUL), OP_MINUS);
//    std::cout << e->evaluate(NULL) << std::endl;
//    delete e; e = NULL;

    
    ExpressionBuilder* b = new ExpressionBuilder("-4/2*3");
    Expression* e = b->getExpression();
    
    std::cout << e->evaluate(NULL) << endl;
    
    SafeDelete(e);
    
    return 0;
}
