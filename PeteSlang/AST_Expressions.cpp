//
//  AST.cpp
//  PeteSlang
//
//  Created by Peter on 29/04/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#include "AST_Expressions.hpp"
#include <math.h>

/*
 * Pure virtul dtor of class Expression
 */
Expression::~Expression() {
}

///--------------------------------------------------------------------------
/*
 * Ctor of BooleanConstant
 */
BooleanConstant::BooleanConstant(bool val_i) {
    pmySymbolInfo = new SymbolInfo();
    pmySymbolInfo->mySymbolName = "";
    pmySymbolInfo->myBoolVal    = val_i;
    pmySymbolInfo->myType       = TYPE_BOOL;
}


/*
 * Dtor of BooleanConstant
 */
BooleanConstant::~BooleanConstant() {
    safe_delete(pmySymbolInfo);
}


/*
 * Evaluation of boolean will given the value
 */
SymbolInfo* BooleanConstant::evaluate(RuntimeContext* context_i) {
    return pmySymbolInfo;
}


/*
 * Function to perform type check
 */
TypeInfo BooleanConstant::typeCheck(CompilationContext* contxt_i) {
    return pmySymbolInfo->myType;
}


/*
 * Function to get the type
 */
TypeInfo BooleanConstant::getType() {
    return pmySymbolInfo->myType;
}


///--------------------------------------------------------------------------
/*
 * Ctor of the class NumericConstant
 */
NumericConstant::NumericConstant(const double value_i) {
    pmySymbolInfo = new SymbolInfo();
    pmySymbolInfo->mySymbolName = "";
    pmySymbolInfo->myDblVal     = value_i;
    pmySymbolInfo->myType       = TYPE_NUMERIC;
}


/*
 * Dtor of the class NumericConstant
 */
NumericConstant::~NumericConstant() {
    safe_delete(pmySymbolInfo);
}


/*
 * Function to evaluate a numeric constant
 */
SymbolInfo* NumericConstant::evaluate(RuntimeContext* context_i) {
    return pmySymbolInfo;
}


/*
 * Function to perform type check
 */
TypeInfo NumericConstant::typeCheck(CompilationContext* contxt_i) {
    return pmySymbolInfo->myType;
}


/*
 * Function to get the type
 */
TypeInfo NumericConstant::getType() {
    return pmySymbolInfo->myType;
}


///--------------------------------------------------------------------------
/*
 * Ctor of the class StringLiteral
 */
StringLiteral::StringLiteral(std::string val_i) {
    pmySymbolInfo = new SymbolInfo();
    pmySymbolInfo->mySymbolName = "";
    pmySymbolInfo->myStrVal     = val_i;
    pmySymbolInfo->myType       = TYPE_STRING;
}


/*
 * Dtor of the class StringLiteral
 */
StringLiteral::~StringLiteral() {
    safe_delete(pmySymbolInfo);
}


/*
 * Function to evaluate a string literal
 */
SymbolInfo* StringLiteral::evaluate(RuntimeContext* context_i) {
    return pmySymbolInfo;
}


/*
 * Function to perform type check
 */
TypeInfo StringLiteral::typeCheck(CompilationContext* contxt_i) {
    return pmySymbolInfo->myType;
}


/*
 * Function to get the type
 */
TypeInfo StringLiteral::getType() {
    return pmySymbolInfo->myType;
}


///--------------------------------------------------------------------------
/*
 * Ctor of the class Variable
 */
Variable::Variable(SymbolInfo* info_i) {
    myVariableName = info_i->mySymbolName;
    myTypeInfo     = info_i->myType;
}


/*
 * Ctor of the class Variable (NUMERIC)
 */
Variable::Variable(CompilationContext* context_i, std::string name_i, double value_i) {
    SymbolInfo* sInfo = new  SymbolInfo();
    sInfo->mySymbolName = name_i;
    sInfo->myDblVal     = value_i;
    myTypeInfo          = TYPE_NUMERIC;
    context_i->addInfo(sInfo);
    myVariableName = name_i;
}


/*
 * Ctor of the class Variable
 */
Variable::Variable(CompilationContext* context_i, std::string name_i, bool value_i) {
    SymbolInfo* sInfo = new  SymbolInfo();
    sInfo->mySymbolName = name_i;
    sInfo->myBoolVal    = value_i;
    myTypeInfo          = TYPE_BOOL;
    context_i->addInfo(sInfo);
    myVariableName = name_i;
}


/*
 * Ctor of the class Variable
 */
Variable::Variable(CompilationContext* context_i, std::string name_i, std::string value_i) {
    SymbolInfo* sInfo = new  SymbolInfo();
    sInfo->mySymbolName = name_i;
    sInfo->myStrVal     = value_i;
    myTypeInfo          = TYPE_STRING;
    context_i->addInfo(sInfo);
    myVariableName = name_i;
}


/*
 * Dtor of the class StringLiteral
 */
Variable::~Variable() {
}


/*
 * To evaluate a variable we just need to
 * look up in the Symbol of RuntimeContext
 */
SymbolInfo* Variable::evaluate(RuntimeContext* context_i) {
    if(nullptr == context_i->getSymbolTable()) {
        return nullptr;
    } else {
        SymbolInfo* info = context_i->getInfo(myVariableName);
        return info;
    }
}


/*
 * Look up in the Symbol table and return the type info
 */
TypeInfo Variable::typeCheck(CompilationContext* context_i) {
    TypeInfo retVal = TYPE_ILLEGAL;
    
    if(nullptr != context_i->getSymbolTable()) {
        SymbolInfo* info = context_i->getInfo(myVariableName);
        
        if(nullptr != info) {
            myTypeInfo = info->myType;
            retVal = myTypeInfo;
        }
    }
    return retVal;
}


/*
 * This function should only be called after the 
 * TypeCheck method has been invoked on AST.
 */
TypeInfo Variable::getType() {
    return myTypeInfo;
}


///--------------------------------------------------------------------------
/*
 * Ctor of the class BinaryPlus
 */
BinaryPlus::BinaryPlus(Expression* exp1_i, Expression* exp2_i) : pmyExp1(exp1_i),
                                                                 pmyExp2(exp2_i)
{
}


/*
 * Dtor of the class BinaryPlus
 */
BinaryPlus::~BinaryPlus() {
    
}


/*
 * Function to evaluate a binary '+' operator
 */
SymbolInfo* BinaryPlus::evaluate(RuntimeContext* context_i) {
    SymbolInfo* evaluateLeft = pmyExp1->evaluate(context_i);
    SymbolInfo* evaluateRight = pmyExp2->evaluate(context_i);
    SymbolInfo* retVal = nullptr;
    
    if(TYPE_STRING == evaluateLeft->myType && TYPE_STRING == evaluateRight->myType) {
        retVal = new SymbolInfo();
        retVal->myStrVal     = evaluateLeft->myStrVal + evaluateRight->myStrVal;
        retVal->myType       = TYPE_STRING;
        retVal->mySymbolName = "";
    } else if (TYPE_NUMERIC == evaluateLeft->myType && TYPE_NUMERIC == evaluateRight->myType) {
        retVal = new SymbolInfo();
        retVal->myDblVal     = evaluateLeft->myDblVal + evaluateRight->myDblVal;
        retVal->myType       = TYPE_NUMERIC;
        retVal->mySymbolName = "";
    } else {
        exit_with_message("Type mismatch");
    }
    
    return retVal;
}


/*
 * Function to perform type check on operands of Binary '+'
 */
TypeInfo BinaryPlus::typeCheck(CompilationContext* context_i) {
    TypeInfo evaluateLeft  = pmyExp1->typeCheck(context_i);
    TypeInfo evaluateRight = pmyExp2->typeCheck(context_i);
    TypeInfo retVal        = TYPE_ILLEGAL;
    
    if (TYPE_BOOL != evaluateLeft && TYPE_BOOL != evaluateRight) {
        myTypeInfo = evaluateLeft;
        retVal     = evaluateLeft;
    }  else {
        exit_with_message("Type mismatch");
    }
    
    return retVal;
}


/*
 * Function to return the type of operands
 */
TypeInfo BinaryPlus::getType() {
    return myTypeInfo;
}


///--------------------------------------------------------------------------
/*
 * Ctor of the class BinaryMinus
 */
BinaryMinus::BinaryMinus(Expression* exp1_i, Expression* exp2_i) : pmyExp1(exp1_i),
                                                                   pmyExp2(exp2_i)
{
}


/*
 * Dtor of the class BinaryMinus
 */
BinaryMinus::~BinaryMinus() {
    
}


/*
 * Function to evaluate a binary '-' operator
 */
SymbolInfo* BinaryMinus::evaluate(RuntimeContext* context_i) {
    SymbolInfo* evaluateLeft = pmyExp1->evaluate(context_i);
    SymbolInfo* evaluateRight = pmyExp2->evaluate(context_i);
    SymbolInfo* retVal = nullptr;
    
    if (TYPE_NUMERIC == evaluateLeft->myType && TYPE_NUMERIC == evaluateRight->myType) {
        retVal = new SymbolInfo();
        retVal->myDblVal     = evaluateLeft->myDblVal - evaluateRight->myDblVal;
        retVal->myType       = TYPE_NUMERIC;
        retVal->mySymbolName = "";
    } else {
        exit_with_message("Type mismatch");
    }
    
    return retVal;
}


/*
 * Function to perform type check on operands of Binary '-'
 */
TypeInfo BinaryMinus::typeCheck(CompilationContext* context_i) {
    TypeInfo evaluateLeft  = pmyExp1->typeCheck(context_i);
    TypeInfo evaluateRight = pmyExp2->typeCheck(context_i);
    TypeInfo retVal        = TYPE_ILLEGAL;
    
    if (TYPE_NUMERIC == evaluateLeft && TYPE_NUMERIC == evaluateRight) {
        myTypeInfo = evaluateLeft;
        retVal     = evaluateLeft;
    }  else {
        exit_with_message("Type mismatch");
    }
    
    return retVal;
}


/*
 * Function to return the type of operands
 */
TypeInfo BinaryMinus::getType() {
    return myTypeInfo;
}


///--------------------------------------------------------------------------
/*
 * Ctor of the class Multiply
 */
Multiply::Multiply(Expression* exp1_i, Expression* exp2_i) : pmyExp1(exp1_i),
pmyExp2(exp2_i)
{
}


/*
 * Dtor of the class Multiply
 */
Multiply::~Multiply() {
    
}


/*
 * Function to evaluate a '*' operator
 */
SymbolInfo* Multiply::evaluate(RuntimeContext* context_i) {
    SymbolInfo* evaluateLeft = pmyExp1->evaluate(context_i);
    SymbolInfo* evaluateRight = pmyExp2->evaluate(context_i);
    SymbolInfo* retVal = nullptr;
    
    if (TYPE_NUMERIC == evaluateLeft->myType && TYPE_NUMERIC == evaluateRight->myType) {
        retVal = new SymbolInfo();
        retVal->myDblVal     = evaluateLeft->myDblVal * evaluateRight->myDblVal;
        retVal->myType       = TYPE_NUMERIC;
        retVal->mySymbolName = "";
    } else {
        exit_with_message("Type mismatch");
    }
    
    return retVal;
}


/*
 * Function to perform type check on operands of operator '*'
 */
TypeInfo Multiply::typeCheck(CompilationContext* context_i) {
    TypeInfo evaluateLeft  = pmyExp1->typeCheck(context_i);
    TypeInfo evaluateRight = pmyExp2->typeCheck(context_i);
    TypeInfo retVal        = TYPE_ILLEGAL;
    
    if (TYPE_NUMERIC == evaluateLeft && TYPE_NUMERIC == evaluateRight) {
        myTypeInfo = evaluateLeft;
        retVal     = evaluateLeft;
    }  else {
        exit_with_message("Type mismatch");
    }
    
    return retVal;
}


/*
 * Function to return the type of operands
 */
TypeInfo Multiply::getType() {
    return myTypeInfo;
}



///--------------------------------------------------------------------------
/*
 * Ctor of the class Divide
 */
Divide::Divide(Expression* exp1_i, Expression* exp2_i) : pmyExp1(exp1_i),
pmyExp2(exp2_i)
{
}


/*
 * Dtor of the class Divide
 */
Divide::~Divide() {
    
}


/*
 * Function to evaluate a '/' operator
 */
SymbolInfo* Divide::evaluate(RuntimeContext* context_i) {
    SymbolInfo* evaluateLeft = pmyExp1->evaluate(context_i);
    SymbolInfo* evaluateRight = pmyExp2->evaluate(context_i);
    SymbolInfo* retVal = nullptr;
    
    if (TYPE_NUMERIC == evaluateLeft->myType && TYPE_NUMERIC == evaluateRight->myType) {
        retVal = new SymbolInfo();
        retVal->myDblVal     = evaluateLeft->myDblVal / evaluateRight->myDblVal;
        retVal->myType       = TYPE_NUMERIC;
        retVal->mySymbolName = "";
    } else {
        exit_with_message("Type mismatch");
    }
    
    return retVal;
}


/*
 * Function to perform type check on operands of '/' operator
 */
TypeInfo Divide::typeCheck(CompilationContext* context_i) {
    TypeInfo evaluateLeft  = pmyExp1->typeCheck(context_i);
    TypeInfo evaluateRight = pmyExp2->typeCheck(context_i);
    TypeInfo retVal        = TYPE_ILLEGAL;
    
    if (TYPE_NUMERIC == evaluateLeft && TYPE_NUMERIC == evaluateRight) {
        myTypeInfo = evaluateLeft;
        retVal     = evaluateLeft;
    }  else {
        exit_with_message("Type mismatch");
    }
    
    return retVal;
}


/*
 * Function to return the type of operands
 */
TypeInfo Divide::getType() {
    return myTypeInfo;
}


///--------------------------------------------------------------------------
/*
 * Ctor of the class UnaryPlus
 */
UnaryPlus::UnaryPlus(Expression* exp_i) : pmyExp(exp_i) {
}


/*
 * Dtor of the class UnaryPlus
 */
UnaryPlus::~UnaryPlus() {
    
}


/*
 * Function to evaluate a Unary + operator
 */
SymbolInfo* UnaryPlus::evaluate(RuntimeContext* context_i) {
    SymbolInfo* evaluateVal = pmyExp->evaluate(context_i);
    SymbolInfo* retVal = nullptr;
    
    if (TYPE_NUMERIC == evaluateVal->myType) {
        retVal = new SymbolInfo();
        retVal->myDblVal     = evaluateVal->myDblVal;
        retVal->myType       = TYPE_NUMERIC;
        retVal->mySymbolName = "";
    } else {
        exit_with_message("Type mismatch");
    }
    
    return retVal;
}


/*
 * Function to perform type check on operands of Unary + operator
 */
TypeInfo UnaryPlus::typeCheck(CompilationContext* context_i) {
    TypeInfo evaluateVal  = pmyExp->typeCheck(context_i);
    TypeInfo retVal        = TYPE_ILLEGAL;
    
    if (TYPE_NUMERIC == evaluateVal) {
        myTypeInfo = evaluateVal;
        retVal     = evaluateVal;
    }  else {
        exit_with_message("Type mismatch");
    }
    
    return retVal;
}


/*
 * Function to return the type of operands
 */
TypeInfo UnaryPlus::getType() {
    return myTypeInfo;
}


///--------------------------------------------------------------------------
/*
 * Ctor of the class UnaryMinus
 */
UnaryMinus::UnaryMinus(Expression* exp_i) : pmyExp(exp_i) {
}


/*
 * Dtor of the class UnaryMinus
 */
UnaryMinus::~UnaryMinus() {
    
}


/*
 * Function to evaluate a Unary - operator
 */
SymbolInfo* UnaryMinus::evaluate(RuntimeContext* context_i) {
    SymbolInfo* evaluateVal = pmyExp->evaluate(context_i);
    SymbolInfo* retVal = nullptr;
    
    if (TYPE_NUMERIC == evaluateVal->myType) {
        retVal = new SymbolInfo();
        retVal->myDblVal     = -evaluateVal->myDblVal;
        retVal->myType       = TYPE_NUMERIC;
        retVal->mySymbolName = "";
    } else {
        exit_with_message("Type mismatch");
    }
    
    return retVal;
}


/*
 * Function to perform type check on operands of Unary - operator
 */
TypeInfo UnaryMinus::typeCheck(CompilationContext* context_i) {
    TypeInfo evaluateVal  = pmyExp->typeCheck(context_i);
    TypeInfo retVal        = TYPE_ILLEGAL;
    
    if (TYPE_NUMERIC == evaluateVal) {
        myTypeInfo = evaluateVal;
        retVal     = evaluateVal;
    }  else {
        exit_with_message("Type mismatch");
    }
    
    return retVal;
}


/*
 * Function to return the type of operands
 */
TypeInfo UnaryMinus::getType() {
    return myTypeInfo;
}


///--------------------------------------------------------------------------
/*
 * Ctor of the class RelationalExpression
 */
RelationalExpression::RelationalExpression(Expression* exp1_i,
                                           Expression* exp2_i,
                                           RelationalOperator opearator_i) : pmyExp1(exp1_i),
                                                                             pmyExp2(exp2_i),
                                                                             myOperator(opearator_i) {
}


/*
 * Dtor of the class RelationalExpression
 */
RelationalExpression::~RelationalExpression() {
}


/*
 * Function to evaluate all the PeteSlang supported relational operators
 */
SymbolInfo* RelationalExpression::evaluate(RuntimeContext* context_i) {
    SymbolInfo* leftInfo  = pmyExp1->evaluate(context_i);
    SymbolInfo* rightInfo = pmyExp2->evaluate(context_i);
    
    if (leftInfo->myType != rightInfo->myType) {
        return nullptr;
    }
    
    SymbolInfo* pRetSymbol = new SymbolInfo();
    pRetSymbol->myType = TYPE_BOOL;
    pRetSymbol->mySymbolName = "";
    
    switch (leftInfo->myType) {
        case TYPE_NUMERIC: {
            if (myOperator == REL_OP_EQ) {
                pRetSymbol->myBoolVal = leftInfo->myDblVal == rightInfo->myDblVal;
            } else if (myOperator == REL_OP_NEQ) {
                pRetSymbol->myBoolVal = leftInfo->myDblVal != rightInfo->myDblVal;
            } else if (myOperator == REL_OP_GT) {
                pRetSymbol->myBoolVal = leftInfo->myDblVal > rightInfo->myDblVal;
            } else if (myOperator == REL_OP_LT) {
                pRetSymbol->myBoolVal = leftInfo->myDblVal < rightInfo->myDblVal;
            } else if (myOperator == REL_OP_GTE) {
                pRetSymbol->myBoolVal = leftInfo->myDblVal >= rightInfo->myDblVal;
            } else if (myOperator == REL_OP_LTE) {
                pRetSymbol->myBoolVal = leftInfo->myDblVal <= rightInfo->myDblVal;
            } else {
                pRetSymbol->myBoolVal = false;
            }
            break;
        }
        
        case TYPE_STRING:{
            if (myOperator == REL_OP_EQ) {
                pRetSymbol->myBoolVal = (0 == leftInfo->myStrVal.compare(rightInfo->myStrVal));
            } else if (myOperator == REL_OP_NEQ) {
                pRetSymbol->myBoolVal = (0 != leftInfo->myStrVal.compare(rightInfo->myStrVal));
            } else {
                pRetSymbol->myBoolVal = false;
            }
            break;
        }
            
        case TYPE_BOOL: {
            if (myOperator == REL_OP_EQ) {
                pRetSymbol->myBoolVal = leftInfo->myBoolVal == rightInfo->myBoolVal;
            } else if (myOperator == REL_OP_NEQ) {
                pRetSymbol->myBoolVal = leftInfo->myBoolVal != rightInfo->myBoolVal;
            } else {
                pRetSymbol->myBoolVal = false;
            }
            break;
        }
            
        default: {
            safe_delete(pRetSymbol);
            return nullptr;
        }
    }
    
    return pRetSymbol;
}


/*
 * Function to perform type check on relational expressions
 */
TypeInfo RelationalExpression::typeCheck(CompilationContext* contxt_i) {
    TypeInfo leftInfo  = pmyExp1->typeCheck(contxt_i);
    TypeInfo rightInfo = pmyExp2->typeCheck(contxt_i);
    
    if (leftInfo != rightInfo) {
        exit_with_message("Wrong Type in expression");
    }
    
    if (leftInfo == TYPE_STRING && !(REL_OP_EQ == myOperator || REL_OP_NEQ == myOperator)) {
        exit_with_message("Only == amd != supported for string type ");
    }
    
    if (leftInfo == TYPE_BOOL && !(REL_OP_EQ == myOperator || REL_OP_NEQ == myOperator)) {
        exit_with_message("Only == amd != supported for bool type ");
    }
    
    myOperandsType = leftInfo;
    myNodeType = TYPE_BOOL;
    
    return myNodeType;
}


/*
 * Fuction to return the type of the current node
 */
TypeInfo RelationalExpression::getType() {
    return myNodeType;
}


///--------------------------------------------------------------------------
/*
 * Ctor of the class LogicalExpression
 */
LogicalExpression::LogicalExpression(Expression* exp1_i,
                                     Expression* exp2_i,
                                     Token opearator_i) : pmyExp1(exp1_i),
                                                          pmyExp2(exp2_i),
                                                          myOperator(opearator_i) {
}


/*
 * Dtor of the class LogicalExpression
 */
LogicalExpression::~LogicalExpression() {
}


/*
 * Function to evaluate all the PeteSlang supported logical operators
 */
SymbolInfo* LogicalExpression::evaluate(RuntimeContext* context_i) {
    SymbolInfo* leftInfo  = pmyExp1->evaluate(context_i);
    SymbolInfo* rightInfo = pmyExp2->evaluate(context_i);
    
    if ((leftInfo->myType != rightInfo->myType) &&
        (TYPE_BOOL != leftInfo->myType)) {
        return nullptr;
    }
    
    SymbolInfo* pRetSymbol = new SymbolInfo();
    pRetSymbol->myType = TYPE_BOOL;
    pRetSymbol->mySymbolName = "";
    
    if (TOK_AND == myOperator) {
        pRetSymbol->myBoolVal = (leftInfo->myBoolVal && rightInfo->myBoolVal);
    } else if (TOK_OR == myOperator) {
        pRetSymbol->myBoolVal = (leftInfo->myBoolVal || rightInfo->myBoolVal);
    } else {
        pRetSymbol->myBoolVal = false;
    }
    
    return pRetSymbol;
}


/*
 * Function to perform type check on logical expressions
 */
TypeInfo LogicalExpression::typeCheck(CompilationContext* contxt_i) {
    TypeInfo leftInfo  = pmyExp1->typeCheck(contxt_i);
    TypeInfo rightInfo = pmyExp2->typeCheck(contxt_i);
    
    if ((leftInfo == rightInfo) && (leftInfo == TYPE_BOOL)) {
        myNodeType = TYPE_BOOL;
    } else {
        exit_with_message("Wrong Type in expression");
    }
    
    return myNodeType;
}


/*
 * Fuction to return the type of the current node
 */
TypeInfo LogicalExpression::getType() {
    return myNodeType;
}


///--------------------------------------------------------------------------
/*
 * Function to evaluate the logical NOT operator
 */
SymbolInfo* LogicalNot::evaluate(RuntimeContext* context_i) {
    SymbolInfo* leftInfo  = pmyExp->evaluate(context_i);
    
    if (TYPE_BOOL == leftInfo->myType) {
        SymbolInfo* pRetSymbol = new SymbolInfo();
        pRetSymbol->myType = TYPE_BOOL;
        pRetSymbol->mySymbolName = "";
        pRetSymbol->myBoolVal = !leftInfo->myBoolVal;
        return pRetSymbol;
    }
    return nullptr;
}


/*
 * Function to type check on the logical not operator.
 */
TypeInfo LogicalNot::typeCheck(CompilationContext* contxt_i) {
    TypeInfo leftInfo = pmyExp->typeCheck(contxt_i);
    
    if (TYPE_BOOL != leftInfo) {
        exit_with_message("Wrong Type in expression");
    }
    myNodeType = TYPE_BOOL;
    
    return myNodeType;
}


/*
 * Function to evaluate all the PeteSlang supported logical operators.
 */
TypeInfo LogicalNot::getType() {
    return myNodeType;
}
