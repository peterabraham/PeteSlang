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
 * Ctor of the class NumericConstant
 */
NumericConstant::NumericConstant(const double value_i): myValue(value_i) {
}


/*
 * Dtor of the class NumericConstant
 */
NumericConstant::~NumericConstant() {
}


/*
 * Function to evaluate a numeric constant
 */
double NumericConstant::evaluate(const RuntimeContext* context_i) {
    return myValue;
}


/*
 * Ctor of the class BinaryExpression
 */
BinaryExpression::BinaryExpression(Expression* pEx1_i,
                                   Expression* pEx2_i,
                                   const Operator optr_i) : pmyExp1(pEx1_i),
                                                            pmyExp2(pEx2_i),
                                                            myOptr(optr_i)
{
}


/*
 * Dtor of the class BinaryExpression
 */
BinaryExpression::~BinaryExpression() {
    safe_delete(pmyExp1);
    safe_delete(pmyExp2);
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
 * Ctor of the class UnaryExpression
 */
UnaryExpression::UnaryExpression(Expression* pEx_i,
                                 const Operator optr_i): pmyExp(pEx_i),
                                                         myOptr(optr_i)
{
}


/*
 * Dtor of the class UnaryExpression
 */
UnaryExpression::~UnaryExpression() {
    safe_delete(pmyExp);
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
