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
#include "MemoryContext.hpp"

/*
 * Abstract base class to represent a PeteSlang expression
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
 * Node for Boolean Constant { TRUE, FALSE } Value
 */
class BooleanConstant : public Expression {
public:
    BooleanConstant(bool val_i);
    ~BooleanConstant();
    SymbolInfo* evaluate(RuntimeContext* context_i);
    TypeInfo typeCheck(CompilationContext* contxt_i);
    TypeInfo getType();
    
private:
    BooleanConstant(BooleanConstant& exp_i) {}
    BooleanConstant& operator=(const BooleanConstant& exp_i) {return *this;}
    
private:
    SymbolInfo* pmySymbolInfo;
};


/*
 * Node  for Numeric constant.
 */
class NumericConstant : public Expression {
public:
    NumericConstant(const double value_i);
    ~NumericConstant();
    SymbolInfo* evaluate(RuntimeContext* context_i);
    TypeInfo typeCheck(CompilationContext* contxt_i);
    TypeInfo getType();
    
private:
    NumericConstant(NumericConstant& exp_i) {}
    NumericConstant& operator=(const NumericConstant& exp_i) {return *this;}
    
private:
    SymbolInfo* pmySymbolInfo;
};


/*
 * To Store Literal string enclosed in quotes
 */
class StringLiteral : public Expression {
public:
    StringLiteral(std::string val_i);
    ~StringLiteral();
    SymbolInfo* evaluate(RuntimeContext* context_i);
    TypeInfo typeCheck(CompilationContext* contxt_i);
    TypeInfo getType();
    
private:
    StringLiteral(StringLiteral& exp_i) {}
    StringLiteral& operator=(const StringLiteral& exp_i) {return *this;}
    
private:
    SymbolInfo* pmySymbolInfo;
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
    ~Variable();
    SymbolInfo* evaluate(RuntimeContext* context_i);
    TypeInfo typeCheck(CompilationContext* contxt_i);
    TypeInfo getType();
    std::string getName() {return myVariableName;}
    
private:
    Variable(Variable& exp) {}
    Variable& operator=(const Variable& exp_i) {return *this;}

private:
    std::string myVariableName;
    TypeInfo    myTypeInfo;
};


/*
 * The node to represent Binary +
 */
class BinaryPlus : public Expression {
public:
    BinaryPlus(Expression* exp1_i, Expression* exp2_i);
    ~BinaryPlus();
    SymbolInfo* evaluate(RuntimeContext* context_i);
    TypeInfo typeCheck(CompilationContext* contxt_i);
    TypeInfo getType();
    
private:
    BinaryPlus(BinaryPlus& exp) {}
    BinaryPlus& operator=(const BinaryPlus& exp_i) {return *this;}
    
private:
    Expression *pmyExp1, *pmyExp2;
    TypeInfo   myTypeInfo;
};



/*
 * The node to represent Binary -
 */
class BinaryMinus : public Expression {
public:
    BinaryMinus(Expression* exp1_i, Expression* exp2_i);
    ~BinaryMinus();
    SymbolInfo* evaluate(RuntimeContext* context_i);
    TypeInfo typeCheck(CompilationContext* contxt_i);
    TypeInfo getType();
    
private:
    BinaryMinus(BinaryMinus& exp) {}
    BinaryMinus& operator=(const BinaryMinus& exp_i) {return *this;}
    
private:
    Expression *pmyExp1, *pmyExp2;
    TypeInfo   myTypeInfo;
};


/*
 * Node to represent Multiplication operation
 */
class Multiply : public Expression {
public:
    Multiply(Expression* exp1_i, Expression* exp2_i);
    ~Multiply();
    SymbolInfo* evaluate(RuntimeContext* context_i);
    TypeInfo typeCheck(CompilationContext* contxt_i);
    TypeInfo getType();
    
private:
    Multiply(Multiply& exp) {}
    Multiply& operator=(const Multiply& exp_i) {return *this;}
    
private:
    Expression *pmyExp1, *pmyExp2;
    TypeInfo   myTypeInfo;
};


/*
 * Node to represent Division operation
 */
class Divide : public Expression {
public:
    Divide(Expression* exp1_i, Expression* exp2_i);
    ~Divide();
    SymbolInfo* evaluate(RuntimeContext* context_i);
    TypeInfo typeCheck(CompilationContext* contxt_i);
    TypeInfo getType();
    
private:
    Divide(Divide& exp) {}
    Divide& operator=(const Divide& exp_i) {return *this;}
    
private:
    Expression *pmyExp1, *pmyExp2;
    TypeInfo   myTypeInfo;
};


/*
 * The node to represent Unary +
 */
class UnaryPlus : public Expression {
public:
    UnaryPlus(Expression* exp1_i);
    ~UnaryPlus();
    SymbolInfo* evaluate(RuntimeContext* context_i);
    TypeInfo typeCheck(CompilationContext* contxt_i);
    TypeInfo getType();
    
private:
    UnaryPlus(UnaryPlus& exp) {}
    UnaryPlus& operator=(const UnaryPlus& exp_i) {return *this;}
    
private:
    Expression *pmyExp;
    TypeInfo   myTypeInfo;
};


/*
 * The node to represent Unary -
 */
class UnaryMinus : public Expression {
public:
    UnaryMinus(Expression* exp1_i);
    ~UnaryMinus();
    SymbolInfo* evaluate(RuntimeContext* context_i);
    TypeInfo typeCheck(CompilationContext* contxt_i);
    TypeInfo getType();
    
private:
    UnaryMinus(UnaryMinus& exp) {}
    UnaryMinus& operator=(const UnaryMinus& exp_i) {return *this;}
    
private:
    Expression *pmyExp;
    TypeInfo   myTypeInfo;
};


/*
 * The node to represent a relational expression
 */
class RelationalExpression : public Expression {
public:
    RelationalExpression(Expression* exp1_i, Expression* exp2_i, RelationalOperator opearator_i);
    ~RelationalExpression();
    SymbolInfo* evaluate(RuntimeContext* context_i);
    TypeInfo typeCheck(CompilationContext* contxt_i);
    TypeInfo getType();
    
private:
    RelationalExpression(RelationalExpression& exp) {}
    RelationalExpression& operator=(const RelationalExpression& exp_i) {return *this;}
    
private:
    Expression* pmyExp1;
    Expression* pmyExp2;
    RelationalOperator myOperator;
    TypeInfo myNodeType;
    TypeInfo myOperandsType;
};


/*
 * The node to represent a logical expression
 */
class LogicalExpression : public Expression {
public:
    LogicalExpression(Expression* exp1_i, Expression* exp2_i, Token opearator_i);
    ~LogicalExpression();
    SymbolInfo* evaluate(RuntimeContext* context_i);
    TypeInfo typeCheck(CompilationContext* contxt_i);
    TypeInfo getType();
    
private:
    LogicalExpression(LogicalExpression& exp) {}
    LogicalExpression& operator=(const LogicalExpression& exp_i) {return *this;}
    
private:
    Expression* pmyExp1;
    Expression* pmyExp2;
    Token myOperator;
    TypeInfo myNodeType;
};


/*
 * The node to represent a logical NOT
 */
class LogicalNot : public Expression {
public:
    LogicalNot(Expression* exp1_i) : pmyExp(exp1_i) {}
    ~LogicalNot() {}
    SymbolInfo* evaluate(RuntimeContext* context_i);
    TypeInfo typeCheck(CompilationContext* contxt_i);
    TypeInfo getType();
    
private:
    LogicalNot(LogicalNot& exp) {}
    LogicalNot& operator=(const LogicalNot& exp_i) {return *this;}
    
private:
    Expression* pmyExp;
    TypeInfo myNodeType;
};
#endif /* AST_hpp */
