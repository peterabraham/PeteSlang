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
SymbolInfo* PrintStatement::execute(RuntimeContext* context_i) {
    SymbolInfo* retVal = pmyExp->evaluate(context_i);
    
    switch (retVal->myType) {
        case TYPE_NUMERIC:
            std::cout << retVal->myDblVal;
            break;
            
        case TYPE_STRING:
            std::cout << retVal->myStrVal;
            break;
            
        case TYPE_BOOL:
            std::cout << (retVal->myBoolVal ? "TRUE" : "FALSE");
            break;
        default:
            break;
    }
    
    return nullptr;
}


///-------------------------------------------------------------------
/*
 * Ctor of PrintLineStatement class
 */
PrintLineStatement::PrintLineStatement(Expression* exp_i) {
    pmyExp = exp_i;
}


/*
 * Dtor of PrintLineStatement class
 */
PrintLineStatement::~PrintLineStatement() {
    safe_delete(pmyExp);
}


/*
 * This print function appends std::endl with
 * std::cout to emit an extra new line.
 */
SymbolInfo* PrintLineStatement::execute(RuntimeContext* context_i) {
    SymbolInfo* retVal = pmyExp->evaluate(context_i);
    
    switch (retVal->myType) {
        case TYPE_NUMERIC:
            std::cout << retVal->myDblVal;
            break;
            
        case TYPE_STRING:
            std::cout << retVal->myStrVal;
            break;
            
        case TYPE_BOOL:
            std::cout << (retVal->myBoolVal ? "TRUE" : "FALSE");
            break;
        default:
            break;
    }
    std::cout << std::endl;
    
    return nullptr;
}


///-------------------------------------------------------------------
/*
 * Ctor of AssignmentStatement class
 */
AssignmentStatement::AssignmentStatement(Variable* var_i, Expression* exp_i) : pmyVar(var_i), pmyExp(exp_i){
}


/*
 * Ctor of AssignmentStatement class
 */
AssignmentStatement::AssignmentStatement(SymbolInfo* info_i, Expression* exp_i) {
    pmyVar = new Variable(info_i);
    pmyExp = exp_i;
}


/*
 * Dtor of AssignmentStatement class
 */
AssignmentStatement::~AssignmentStatement() {
    //safe_delete(pmyVar);
}


/*
 * Function to evaluate and execute assignment operator
 */
SymbolInfo* AssignmentStatement::execute(RuntimeContext* context_i) {
    SymbolInfo* value = pmyExp->evaluate(context_i);
    context_i->assignInfo(pmyVar, value);
    
    return value;
}


///-------------------------------------------------------------------
/*
 * Ctor of VariableDeclStatement class
 */
VariableDeclStatement::VariableDeclStatement(SymbolInfo* symbolInfo_i) : pmySymbolInfo(symbolInfo_i) {
}


/*
 * Dtor of VariableDeclStatement class
 */
VariableDeclStatement::~VariableDeclStatement() {
    safe_delete(pmyVar);
}


/*
 * Function to evaluate and execute variable declaration
 */
SymbolInfo* VariableDeclStatement::execute(RuntimeContext* context_i) {
    context_i->addInfo(pmySymbolInfo);
    pmyVar = new Variable(pmySymbolInfo);
    
    return nullptr;
}
