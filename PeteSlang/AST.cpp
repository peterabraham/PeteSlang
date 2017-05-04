//
//  AST.cpp
//  PeteSlang
//
//  Created by Peter on 29/04/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#include "AST.hpp"
#include <math.h>

/*
 * Pure virtul dtor of class Expression
 */
Expression::~Expression() {
}


/*
 * Function to evaluate a numeric constant
 */
double NumericConstant::evaluate(const RuntimeContext* context_i) {
    return myValue;
}


/*
 * Function to evaluate a binary expression
 */
double BinaryExpression::evaluate(const RuntimeContext* context_i) {
    switch (myOptr) {
        case OP_PLUS:
        return pmyExp1->evaluate(context_i) + pmyExp2->evaluate(context_i);
        break;
        
        case OP_MINUS:
        return pmyExp1->evaluate(context_i) - pmyExp2->evaluate(context_i);
        break;
        
        case OP_MUL:
        return pmyExp1->evaluate(context_i) * pmyExp2->evaluate(context_i);
        break;
        
        case OP_DIV:
        return pmyExp1->evaluate(context_i) / pmyExp2->evaluate(context_i);
        break;
        
        default:
        break;
    }
    
    return NAN;
}


/*
 * Function to evaluate a unary expression
 */
double UnaryExpression::evaluate(const RuntimeContext* context_i) {
    switch (myOptr) {
        case OP_PLUS:
        return pmyExp->evaluate(context_i);
        break;
        
        case OP_MINUS:
        return -pmyExp->evaluate(context_i);
        
        default:
        break;
    }
    
    return NAN;
}
