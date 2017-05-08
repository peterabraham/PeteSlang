//
//  AST_Statements.cpp
//  PeteSlang
//
//  Created by Peter on 05/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#include "AST_Statements.hpp"


/*
 * Dtor of Statement class
 */
Statement::~Statement() {
}


/*
 * Ctor of PrintStatement class
 */
PrintStatement::PrintStatement(Expression* exp_i) {
    pmyExp = exp_i;
}


/*
 * Dtor of PrintStatement class
 */
PrintStatement::~PrintStatement() {
    safe_delete(pmyExp);
}


/*
 * This function evaluates the expression and
 * spits the value to the console using
 * std::cout statement.
 */
bool PrintStatement::execute(const RuntimeContext* context_i) {
    double printVal = pmyExp->evaluate(context_i);
    std::cout << printVal;
    return true;
}


/*
 * Ctor of PrintLineStatement class
 */
PrintLineStatement::PrintLineStatement(Expression* exp_i) {
    pmyExp = exp_i;
}


/*
 * Ctor of PrintLineStatement class
 */
PrintLineStatement::~PrintLineStatement() {
    safe_delete(pmyExp);
}


/*
 * This print function appends std::endl with
 * std::cout to emit an extra new line.
 */
bool PrintLineStatement::execute(const RuntimeContext* context_i) {
    double printVal = pmyExp->evaluate(context_i);
    std::cout << printVal << std::endl;
    return true;
}
