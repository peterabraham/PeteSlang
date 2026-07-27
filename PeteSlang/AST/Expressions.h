//
//  Expressions.h
//  PeteSlang
//
//  Created by Peter on 29/04/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#pragma once

#include "Common.h"
#include "Runtime/MemoryContext.h"
#include <memory>

/*
 * Abstract base class to represent a PeteSlang expression.
 *
 * Every child Expression* in this hierarchy is owned via
 * std::unique_ptr (see the members below), so subclasses no longer
 * need a hand-written destructor to delete them, and are
 * automatically non-copyable (unique_ptr members suppress the
 * implicit copy ctor/assignment) instead of relying on the old
 * private-empty-body trick.
 */
class Expression {
public:
    Expression() {}
    virtual ~Expression() = 0;
    virtual SymbolInfo* evaluate(RuntimeContext* context_i) = 0;
    virtual TypeInfo typeCheck(CompilationContext* contxt_i) = 0;
    virtual TypeInfo getType() = 0;
};


/*
 * Node for Boolean Constant { TRUE, FALSE } Value.
 * Owns its SymbolInfo by value (one constant, one result, for the
 * node's whole lifetime) instead of a heap-allocated pointer.
 */
class BooleanConstant : public Expression {
public:
    BooleanConstant(bool val_i);
    SymbolInfo* evaluate(RuntimeContext* context_i) override;
    TypeInfo typeCheck(CompilationContext* contxt_i) override;
    TypeInfo getType() override;

    BooleanConstant(const BooleanConstant&) = delete;
    BooleanConstant& operator=(const BooleanConstant&) = delete;

private:
    SymbolInfo mySymbolInfo;
};


/*
 * Node  for Numeric constant.
 */
class NumericConstant : public Expression {
public:
    NumericConstant(double value_i);
    SymbolInfo* evaluate(RuntimeContext* context_i) override;
    TypeInfo typeCheck(CompilationContext* contxt_i) override;
    TypeInfo getType() override;

    NumericConstant(const NumericConstant&) = delete;
    NumericConstant& operator=(const NumericConstant&) = delete;

private:
    SymbolInfo mySymbolInfo;
};


/*
 * To Store Literal string enclosed in quotes
 */
class StringLiteral : public Expression {
public:
    StringLiteral(std::string val_i);
    SymbolInfo* evaluate(RuntimeContext* context_i) override;
    TypeInfo typeCheck(CompilationContext* contxt_i) override;
    TypeInfo getType() override;

    StringLiteral(const StringLiteral&) = delete;
    StringLiteral& operator=(const StringLiteral&) = delete;

private:
    SymbolInfo mySymbolInfo;
};


/*
 * Node to store Variables.
 * The data types supported are NUMERIC, STRING, BOOLEAN.
 * The node store only the variable name,
 * the associated data will be found in the
 * Symbol Table attached to the COMPILATION_CONTEXT
 */
class Variable : public Expression {
public:
    Variable(SymbolInfo* info_i);
    Variable(CompilationContext* context_i, std::string name_i, double value_i);
    Variable(CompilationContext* context_i, std::string name_i, bool value_i);
    Variable(CompilationContext* context_i, std::string name_i, std::string value_i);
    SymbolInfo* evaluate(RuntimeContext* context_i) override;
    TypeInfo typeCheck(CompilationContext* contxt_i) override;
    TypeInfo getType() override;
    std::string getName() {return myVariableName;}

    Variable(const Variable&) = delete;
    Variable& operator=(const Variable&) = delete;

private:
    std::string myVariableName;
    TypeInfo    myTypeInfo;
};


/*
 * The node to represent Binary +
 */
class BinaryPlus : public Expression {
public:
    BinaryPlus(std::unique_ptr<Expression> exp1_i, std::unique_ptr<Expression> exp2_i);
    SymbolInfo* evaluate(RuntimeContext* context_i) override;
    TypeInfo typeCheck(CompilationContext* contxt_i) override;
    TypeInfo getType() override;

private:
    std::unique_ptr<Expression> pmyExp1, pmyExp2;
    TypeInfo   myTypeInfo;
};


/*
 * The node to represent Binary -
 */
class BinaryMinus : public Expression {
public:
    BinaryMinus(std::unique_ptr<Expression> exp1_i, std::unique_ptr<Expression> exp2_i);
    SymbolInfo* evaluate(RuntimeContext* context_i) override;
    TypeInfo typeCheck(CompilationContext* contxt_i) override;
    TypeInfo getType() override;

private:
    std::unique_ptr<Expression> pmyExp1, pmyExp2;
    TypeInfo   myTypeInfo;
};


/*
 * Node to represent Multiplication operation
 */
class Multiply : public Expression {
public:
    Multiply(std::unique_ptr<Expression> exp1_i, std::unique_ptr<Expression> exp2_i);
    SymbolInfo* evaluate(RuntimeContext* context_i) override;
    TypeInfo typeCheck(CompilationContext* contxt_i) override;
    TypeInfo getType() override;

private:
    std::unique_ptr<Expression> pmyExp1, pmyExp2;
    TypeInfo   myTypeInfo;
};


/*
 * Node to represent Division operation
 */
class Divide : public Expression {
public:
    Divide(std::unique_ptr<Expression> exp1_i, std::unique_ptr<Expression> exp2_i);
    SymbolInfo* evaluate(RuntimeContext* context_i) override;
    TypeInfo typeCheck(CompilationContext* contxt_i) override;
    TypeInfo getType() override;

private:
    std::unique_ptr<Expression> pmyExp1, pmyExp2;
    TypeInfo   myTypeInfo;
};


/*
 * The node to represent Unary +
 */
class UnaryPlus : public Expression {
public:
    UnaryPlus(std::unique_ptr<Expression> exp1_i);
    SymbolInfo* evaluate(RuntimeContext* context_i) override;
    TypeInfo typeCheck(CompilationContext* contxt_i) override;
    TypeInfo getType() override;

private:
    std::unique_ptr<Expression> pmyExp;
    TypeInfo   myTypeInfo;
};


/*
 * The node to represent Unary -
 */
class UnaryMinus : public Expression {
public:
    UnaryMinus(std::unique_ptr<Expression> exp1_i);
    SymbolInfo* evaluate(RuntimeContext* context_i) override;
    TypeInfo typeCheck(CompilationContext* contxt_i) override;
    TypeInfo getType() override;

private:
    std::unique_ptr<Expression> pmyExp;
    TypeInfo   myTypeInfo;
};


/*
 * The node to represent a relational expression
 */
class RelationalExpression : public Expression {
public:
    RelationalExpression(std::unique_ptr<Expression> exp1_i, std::unique_ptr<Expression> exp2_i, RelationalOperator opearator_i);
    SymbolInfo* evaluate(RuntimeContext* context_i) override;
    TypeInfo typeCheck(CompilationContext* contxt_i) override;
    TypeInfo getType() override;

private:
    std::unique_ptr<Expression> pmyExp1;
    std::unique_ptr<Expression> pmyExp2;
    RelationalOperator myOperator;
    TypeInfo myNodeType;
    TypeInfo myOperandsType;
};


/*
 * The node to represent a logical expression
 */
class LogicalExpression : public Expression {
public:
    LogicalExpression(std::unique_ptr<Expression> exp1_i, std::unique_ptr<Expression> exp2_i, Token opearator_i);
    SymbolInfo* evaluate(RuntimeContext* context_i) override;
    TypeInfo typeCheck(CompilationContext* contxt_i) override;
    TypeInfo getType() override;

private:
    std::unique_ptr<Expression> pmyExp1;
    std::unique_ptr<Expression> pmyExp2;
    Token myOperator;
    TypeInfo myNodeType;
};


/*
 * The node to represent a logical NOT
 */
class LogicalNot : public Expression {
public:
    LogicalNot(std::unique_ptr<Expression> exp1_i) : pmyExp(std::move(exp1_i)) {}
    SymbolInfo* evaluate(RuntimeContext* context_i) override;
    TypeInfo typeCheck(CompilationContext* contxt_i) override;
    TypeInfo getType() override;

private:
    std::unique_ptr<Expression> pmyExp;
    TypeInfo myNodeType;
};
