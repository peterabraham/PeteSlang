//
//  Expressions.cpp
//  PeteSlang
//
//  Created by Peter on 29/04/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#include "AST/Expressions.h"
#include <math.h>
#include <utility>

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
    mySymbolInfo.mySymbolName = "";
    mySymbolInfo.setBool(val_i);
    mySymbolInfo.myType       = TypeInfo::TYPE_BOOL;
}


/*
 * Evaluation of boolean will given the value
 */
SymbolInfo* BooleanConstant::evaluate(RuntimeContext* context_i) {
    return &mySymbolInfo;
}


/*
 * Function to perform type check
 */
TypeInfo BooleanConstant::typeCheck(CompilationContext* contxt_i) {
    return mySymbolInfo.myType;
}


/*
 * Function to get the type
 */
TypeInfo BooleanConstant::getType() {
    return mySymbolInfo.myType;
}


///--------------------------------------------------------------------------
/*
 * Ctor of the class NumericConstant
 */
NumericConstant::NumericConstant(double value_i) {
    mySymbolInfo.mySymbolName = "";
    mySymbolInfo.setDouble(value_i);
    mySymbolInfo.myType       = TypeInfo::TYPE_NUMERIC;
}


/*
 * Function to evaluate a numeric constant
 */
SymbolInfo* NumericConstant::evaluate(RuntimeContext* context_i) {
    return &mySymbolInfo;
}


/*
 * Function to perform type check
 */
TypeInfo NumericConstant::typeCheck(CompilationContext* contxt_i) {
    return mySymbolInfo.myType;
}


/*
 * Function to get the type
 */
TypeInfo NumericConstant::getType() {
    return mySymbolInfo.myType;
}


///--------------------------------------------------------------------------
/*
 * Ctor of the class StringLiteral
 */
StringLiteral::StringLiteral(std::string val_i) {
    mySymbolInfo.mySymbolName = "";
    mySymbolInfo.setString(std::move(val_i));
    mySymbolInfo.myType       = TypeInfo::TYPE_STRING;
}


/*
 * Function to evaluate a string literal
 */
SymbolInfo* StringLiteral::evaluate(RuntimeContext* context_i) {
    return &mySymbolInfo;
}


/*
 * Function to perform type check
 */
TypeInfo StringLiteral::typeCheck(CompilationContext* contxt_i) {
    return mySymbolInfo.myType;
}


/*
 * Function to get the type
 */
TypeInfo StringLiteral::getType() {
    return mySymbolInfo.myType;
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
    sInfo->setDouble(value_i);
    myTypeInfo          = TypeInfo::TYPE_NUMERIC;
    context_i->addInfo(sInfo);
    myVariableName = name_i;
}


/*
 * Ctor of the class Variable
 */
Variable::Variable(CompilationContext* context_i, std::string name_i, bool value_i) {
    SymbolInfo* sInfo = new  SymbolInfo();
    sInfo->mySymbolName = name_i;
    sInfo->setBool(value_i);
    myTypeInfo          = TypeInfo::TYPE_BOOL;
    context_i->addInfo(sInfo);
    myVariableName = name_i;
}


/*
 * Ctor of the class Variable
 */
Variable::Variable(CompilationContext* context_i, std::string name_i, std::string value_i) {
    SymbolInfo* sInfo = new  SymbolInfo();
    sInfo->mySymbolName = name_i;
    sInfo->setString(value_i);
    myTypeInfo          = TypeInfo::TYPE_STRING;
    context_i->addInfo(sInfo);
    myVariableName = name_i;
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
    TypeInfo retVal = TypeInfo::TYPE_ILLEGAL;

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
BinaryPlus::BinaryPlus(std::unique_ptr<Expression> exp1_i, std::unique_ptr<Expression> exp2_i) : pmyExp1(std::move(exp1_i)),
                                                                 pmyExp2(std::move(exp2_i))
{
}


/*
 * Function to evaluate a binary '+' operator
 */
SymbolInfo* BinaryPlus::evaluate(RuntimeContext* context_i) {
    SymbolInfo* evaluateLeft = pmyExp1->evaluate(context_i);
    SymbolInfo* evaluateRight = pmyExp2->evaluate(context_i);
    SymbolInfo* retVal = nullptr;

    if(TypeInfo::TYPE_STRING == evaluateLeft->myType && TypeInfo::TYPE_STRING == evaluateRight->myType) {
        retVal = new SymbolInfo();
        retVal->setString(evaluateLeft->getString() + evaluateRight->getString());
        retVal->myType       = TypeInfo::TYPE_STRING;
        retVal->mySymbolName = "";
    } else if (TypeInfo::TYPE_NUMERIC == evaluateLeft->myType && TypeInfo::TYPE_NUMERIC == evaluateRight->myType) {
        retVal = new SymbolInfo();
        retVal->setDouble(evaluateLeft->getDouble() + evaluateRight->getDouble());
        retVal->myType       = TypeInfo::TYPE_NUMERIC;
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
    TypeInfo retVal        = TypeInfo::TYPE_ILLEGAL;

    if (TypeInfo::TYPE_BOOL != evaluateLeft && TypeInfo::TYPE_BOOL != evaluateRight) {
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
BinaryMinus::BinaryMinus(std::unique_ptr<Expression> exp1_i, std::unique_ptr<Expression> exp2_i) : pmyExp1(std::move(exp1_i)),
                                                                   pmyExp2(std::move(exp2_i))
{
}


/*
 * Function to evaluate a binary '-' operator
 */
SymbolInfo* BinaryMinus::evaluate(RuntimeContext* context_i) {
    SymbolInfo* evaluateLeft = pmyExp1->evaluate(context_i);
    SymbolInfo* evaluateRight = pmyExp2->evaluate(context_i);
    SymbolInfo* retVal = nullptr;

    if (TypeInfo::TYPE_NUMERIC == evaluateLeft->myType && TypeInfo::TYPE_NUMERIC == evaluateRight->myType) {
        retVal = new SymbolInfo();
        retVal->setDouble(evaluateLeft->getDouble() - evaluateRight->getDouble());
        retVal->myType       = TypeInfo::TYPE_NUMERIC;
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
    TypeInfo retVal        = TypeInfo::TYPE_ILLEGAL;

    if (TypeInfo::TYPE_NUMERIC == evaluateLeft && TypeInfo::TYPE_NUMERIC == evaluateRight) {
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
Multiply::Multiply(std::unique_ptr<Expression> exp1_i, std::unique_ptr<Expression> exp2_i) : pmyExp1(std::move(exp1_i)),
pmyExp2(std::move(exp2_i))
{
}


/*
 * Function to evaluate a '*' operator
 */
SymbolInfo* Multiply::evaluate(RuntimeContext* context_i) {
    SymbolInfo* evaluateLeft = pmyExp1->evaluate(context_i);
    SymbolInfo* evaluateRight = pmyExp2->evaluate(context_i);
    SymbolInfo* retVal = nullptr;

    if (TypeInfo::TYPE_NUMERIC == evaluateLeft->myType && TypeInfo::TYPE_NUMERIC == evaluateRight->myType) {
        retVal = new SymbolInfo();
        retVal->setDouble(evaluateLeft->getDouble() * evaluateRight->getDouble());
        retVal->myType       = TypeInfo::TYPE_NUMERIC;
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
    TypeInfo retVal        = TypeInfo::TYPE_ILLEGAL;

    if (TypeInfo::TYPE_NUMERIC == evaluateLeft && TypeInfo::TYPE_NUMERIC == evaluateRight) {
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
Divide::Divide(std::unique_ptr<Expression> exp1_i, std::unique_ptr<Expression> exp2_i) : pmyExp1(std::move(exp1_i)),
pmyExp2(std::move(exp2_i))
{
}


/*
 * Function to evaluate a '/' operator
 */
SymbolInfo* Divide::evaluate(RuntimeContext* context_i) {
    SymbolInfo* evaluateLeft = pmyExp1->evaluate(context_i);
    SymbolInfo* evaluateRight = pmyExp2->evaluate(context_i);
    SymbolInfo* retVal = nullptr;

    if (TypeInfo::TYPE_NUMERIC == evaluateLeft->myType && TypeInfo::TYPE_NUMERIC == evaluateRight->myType) {
        retVal = new SymbolInfo();
        retVal->setDouble(evaluateLeft->getDouble() / evaluateRight->getDouble());
        retVal->myType       = TypeInfo::TYPE_NUMERIC;
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
    TypeInfo retVal        = TypeInfo::TYPE_ILLEGAL;

    if (TypeInfo::TYPE_NUMERIC == evaluateLeft && TypeInfo::TYPE_NUMERIC == evaluateRight) {
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
UnaryPlus::UnaryPlus(std::unique_ptr<Expression> exp_i) : pmyExp(std::move(exp_i)) {
}


/*
 * Function to evaluate a Unary + operator
 */
SymbolInfo* UnaryPlus::evaluate(RuntimeContext* context_i) {
    SymbolInfo* evaluateVal = pmyExp->evaluate(context_i);
    SymbolInfo* retVal = nullptr;

    if (TypeInfo::TYPE_NUMERIC == evaluateVal->myType) {
        retVal = new SymbolInfo();
        retVal->setDouble(evaluateVal->getDouble());
        retVal->myType       = TypeInfo::TYPE_NUMERIC;
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
    TypeInfo retVal        = TypeInfo::TYPE_ILLEGAL;

    if (TypeInfo::TYPE_NUMERIC == evaluateVal) {
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
UnaryMinus::UnaryMinus(std::unique_ptr<Expression> exp_i) : pmyExp(std::move(exp_i)) {
}


/*
 * Function to evaluate a Unary - operator
 */
SymbolInfo* UnaryMinus::evaluate(RuntimeContext* context_i) {
    SymbolInfo* evaluateVal = pmyExp->evaluate(context_i);
    SymbolInfo* retVal = nullptr;

    if (TypeInfo::TYPE_NUMERIC == evaluateVal->myType) {
        retVal = new SymbolInfo();
        retVal->setDouble(-evaluateVal->getDouble());
        retVal->myType       = TypeInfo::TYPE_NUMERIC;
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
    TypeInfo retVal        = TypeInfo::TYPE_ILLEGAL;

    if (TypeInfo::TYPE_NUMERIC == evaluateVal) {
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
RelationalExpression::RelationalExpression(std::unique_ptr<Expression> exp1_i,
                                           std::unique_ptr<Expression> exp2_i,
                                           RelationalOperator opearator_i) : pmyExp1(std::move(exp1_i)),
                                                                             pmyExp2(std::move(exp2_i)),
                                                                             myOperator(opearator_i) {
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
    pRetSymbol->myType = TypeInfo::TYPE_BOOL;
    pRetSymbol->mySymbolName = "";

    switch (leftInfo->myType) {
        case TypeInfo::TYPE_NUMERIC: {
            if (myOperator == RelationalOperator::REL_OP_EQ) {
                pRetSymbol->setBool(leftInfo->getDouble() == rightInfo->getDouble());
            } else if (myOperator == RelationalOperator::REL_OP_NEQ) {
                pRetSymbol->setBool(leftInfo->getDouble() != rightInfo->getDouble());
            } else if (myOperator == RelationalOperator::REL_OP_GT) {
                pRetSymbol->setBool(leftInfo->getDouble() > rightInfo->getDouble());
            } else if (myOperator == RelationalOperator::REL_OP_LT) {
                pRetSymbol->setBool(leftInfo->getDouble() < rightInfo->getDouble());
            } else if (myOperator == RelationalOperator::REL_OP_GTE) {
                pRetSymbol->setBool(leftInfo->getDouble() >= rightInfo->getDouble());
            } else if (myOperator == RelationalOperator::REL_OP_LTE) {
                pRetSymbol->setBool(leftInfo->getDouble() <= rightInfo->getDouble());
            } else {
                pRetSymbol->setBool(false);
            }
            break;
        }

        case TypeInfo::TYPE_STRING:{
            if (myOperator == RelationalOperator::REL_OP_EQ) {
                pRetSymbol->setBool(0 == leftInfo->getString().compare(rightInfo->getString()));
            } else if (myOperator == RelationalOperator::REL_OP_NEQ) {
                pRetSymbol->setBool(0 != leftInfo->getString().compare(rightInfo->getString()));
            } else {
                pRetSymbol->setBool(false);
            }
            break;
        }

        case TypeInfo::TYPE_BOOL: {
            if (myOperator == RelationalOperator::REL_OP_EQ) {
                pRetSymbol->setBool(leftInfo->getBool() == rightInfo->getBool());
            } else if (myOperator == RelationalOperator::REL_OP_NEQ) {
                pRetSymbol->setBool(leftInfo->getBool() != rightInfo->getBool());
            } else {
                pRetSymbol->setBool(false);
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

    if (leftInfo == TypeInfo::TYPE_STRING && !(RelationalOperator::REL_OP_EQ == myOperator || RelationalOperator::REL_OP_NEQ == myOperator)) {
        exit_with_message("Only == amd != supported for string type ");
    }

    if (leftInfo == TypeInfo::TYPE_BOOL && !(RelationalOperator::REL_OP_EQ == myOperator || RelationalOperator::REL_OP_NEQ == myOperator)) {
        exit_with_message("Only == amd != supported for bool type ");
    }

    myOperandsType = leftInfo;
    myNodeType = TypeInfo::TYPE_BOOL;

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
LogicalExpression::LogicalExpression(std::unique_ptr<Expression> exp1_i,
                                     std::unique_ptr<Expression> exp2_i,
                                     Token opearator_i) : pmyExp1(std::move(exp1_i)),
                                                          pmyExp2(std::move(exp2_i)),
                                                          myOperator(opearator_i) {
}


/*
 * Function to evaluate all the PeteSlang supported logical operators
 */
SymbolInfo* LogicalExpression::evaluate(RuntimeContext* context_i) {
    SymbolInfo* leftInfo  = pmyExp1->evaluate(context_i);
    SymbolInfo* rightInfo = pmyExp2->evaluate(context_i);

    if ((leftInfo->myType != rightInfo->myType) &&
        (TypeInfo::TYPE_BOOL != leftInfo->myType)) {
        return nullptr;
    }

    SymbolInfo* pRetSymbol = new SymbolInfo();
    pRetSymbol->myType = TypeInfo::TYPE_BOOL;
    pRetSymbol->mySymbolName = "";

    if (Token::TOK_AND == myOperator) {
        pRetSymbol->setBool(leftInfo->getBool() && rightInfo->getBool());
    } else if (Token::TOK_OR == myOperator) {
        pRetSymbol->setBool(leftInfo->getBool() || rightInfo->getBool());
    } else {
        pRetSymbol->setBool(false);
    }

    return pRetSymbol;
}


/*
 * Function to perform type check on logical expressions
 */
TypeInfo LogicalExpression::typeCheck(CompilationContext* contxt_i) {
    TypeInfo leftInfo  = pmyExp1->typeCheck(contxt_i);
    TypeInfo rightInfo = pmyExp2->typeCheck(contxt_i);

    if ((leftInfo == rightInfo) && (leftInfo == TypeInfo::TYPE_BOOL)) {
        myNodeType = TypeInfo::TYPE_BOOL;
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

    if (TypeInfo::TYPE_BOOL == leftInfo->myType) {
        SymbolInfo* pRetSymbol = new SymbolInfo();
        pRetSymbol->myType = TypeInfo::TYPE_BOOL;
        pRetSymbol->mySymbolName = "";
        pRetSymbol->setBool(!leftInfo->getBool());
        return pRetSymbol;
    }
    return nullptr;
}


/*
 * Function to type check on the logical not operator.
 */
TypeInfo LogicalNot::typeCheck(CompilationContext* contxt_i) {
    TypeInfo leftInfo = pmyExp->typeCheck(contxt_i);

    if (TypeInfo::TYPE_BOOL != leftInfo) {
        exit_with_message("Wrong Type in expression");
    }
    myNodeType = TypeInfo::TYPE_BOOL;

    return myNodeType;
}


/*
 * Function to evaluate all the PeteSlang supported logical operators.
 */
TypeInfo LogicalNot::getType() {
    return myNodeType;
}
