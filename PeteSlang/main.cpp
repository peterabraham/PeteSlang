//
//  main.cpp
//  PeteSlang
//
//  Created by Peter on 29/04/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#include "Builder.hpp"

// Test routine for Step 3
static void test_first_script() {
    string a = "PRINTLINE 2*10;\n\rPRINTLINE 10;\nPRINT 2*10;\r\n";
    Parser* p = new Parser(a);
    vector<Statement*> stmnts = p->parse();
    
    for (std::vector<Statement*>::iterator it = stmnts.begin() ; it != stmnts.end(); ++it) {
        Statement* st = dynamic_cast<Statement*>(*it);
        st->execute(nullptr);
        safe_delete(st);
    }
}



int main(int argc, const char * argv[]) {

/// Step 1
//    NumericConstant c1(5), c2(10), c3(30), c4(50);
//    
//    Expression* e = new BinaryExpression(&c1, &c2, OP_MUL);
//    std::cout << e->evaluate(NULL) << std::endl;
//    delete e; e = NULL;
//                                             
//    e = new UnaryExpression(new BinaryExpression(&c2, new BinaryExpression(&c3, &c4, OP_PLUS), OP_MUL), OP_MINUS);
//    std::cout << e->evaluate(NULL) << std::endl;
//    delete e; e = NULL;

/// Step 2
//    ExpressionBuilder* b = new ExpressionBuilder("-4/2*3");
//    Expression* e = b->getExpression();
//    
//    std::cout << e->evaluate(NULL) << endl;
//    
//    safe_delete(e);
    
/// Step 3
    test_first_script();
    
    return 0;
}


