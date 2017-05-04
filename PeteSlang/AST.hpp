//
//  AST.hpp
//  PeteSlang
//
//  Created by Peter on 29/04/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#ifndef AST_hpp
#define AST_hpp

#include "Common.hpp"
#include "RuntimeContext.hpp"

/*
 *
 */
class Expression {
    
public:
    Expression() {}
    virtual ~Expression() = 0;
    virtual double evaluate(const RuntimeContext* context_i) = 0;
    
private:
    Expression(Expression& exp_i) {}
    Expression& operator=(const Expression& exp_i) {return *this;}
    
};


/*
 *
 */
class NumericConstant : public Expression {
    
public:
    NumericConstant(const double value_i): myValue(value_i) {}
    ~NumericConstant() {}
    double evaluate(const RuntimeContext* context_i);
    
private:
    NumericConstant(NumericConstant& exp_i) {}
    NumericConstant& operator=(const NumericConstant& exp_i) {return *this;}
    
private:
    double myValue;
};


/*
 *
 */
class BinaryExpression : public Expression {
    
public:
    BinaryExpression(Expression* pEx1_i, Expression* pEx2_i, const Operator optr_i): pmyExp1(pEx1_i), pmyExp2(pEx2_i), myOptr(optr_i) {}
    ~BinaryExpression() {}
    double evaluate(const RuntimeContext* context_i);
    
private:
    BinaryExpression(BinaryExpression& exp_i) {}
    BinaryExpression& operator=(const BinaryExpression& exp_i) {return *this;}
    
private:
    Operator myOptr;
    Expression *pmyExp1, *pmyExp2;
};


/*
 *
 */
class UnaryExpression : public Expression {
    
public:
    UnaryExpression(Expression* pEx_i, const Operator optr_i): pmyExp(pEx_i), myOptr(optr_i) {}
    ~UnaryExpression() {}
    double evaluate(const RuntimeContext* context_i);
    
private:
    UnaryExpression(UnaryExpression& exp) {}
    UnaryExpression& operator=(const UnaryExpression& exp_i) {return *this;}

private:
    Operator myOptr;
    Expression *pmyExp;
};


#endif /* AST_hpp */
