//
//  main.cpp
//  PeteSlang
//
//  Created by Peter on 29/04/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#include "Frontend/Builder.h"
#include <fstream>
#include <memory>
#include <streambuf>

// Test routine for Step 4
static void testFileScript(string fileName_i) {
    ifstream inFile;
    inFile.open(fileName_i);//open the input file

    stringstream strStream;
    strStream << inFile.rdbuf();//read the file
    string prog = strStream.str();//str holds the content of the file

    // Creates a Parser object with Program text as argument
    auto parser = std::make_unique<Parser>(prog);

    // Creates a compilation context
    auto context = std::make_unique<CompilationContext>();

    // Call the top level Parsing Routine with
    // Compilation Context as the Argument
    vector<std::unique_ptr<Statement>> stmnts = parser->parse(context.get());

    // if we have reached here , the parse process
    // is successful... Create a Run time context and
    // Call Execute statements of each statement...
    auto runCntxt = std::make_unique<RuntimeContext>();
    for (auto& st : stmnts) {
        st->execute(runCntxt.get());
    }

    inFile.close();
}

//// Test routine for Step 3
//static void test_first_script() {
//    string a = "PRINTLINE 2*10;\n\rPRINTLINE 10;\nPRINT 2*10;\r\n";
//    Parser* p = new Parser(a);
//    vector<Statement*> stmnts = p->parse(nullptr);
//
//    for (std::vector<Statement*>::iterator it = stmnts.begin() ; it != stmnts.end(); ++it) {
//        Statement* st = dynamic_cast<Statement*>(*it);
//        st->execute(nullptr);
//        safe_delete(st);
//    }
//}


/*
 * Entry point for the Compiler(Interpretter actually!)
 */
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
//    test_first_script();

// Step 4
    if(1 >= argc) {
        exit_with_message("PeteSlang <scriptname>\n");
    }

    string file = argv[1];
    testFileScript(file);

    return 1;
}
