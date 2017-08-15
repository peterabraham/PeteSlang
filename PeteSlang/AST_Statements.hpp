//
//  AST_Statements.hpp
//  PeteSlang
//
//  Created by Peter on 05/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#ifndef AST_Statements_hpp
#define AST_Statements_hpp

#include "AST_Expressions.hpp"
#include <vector>

/*
 * Statement is what you Execute for it's Effect
 */
class Statement {
    
public:
    virtual ~Statement() = 0;
    virtual SymbolInfo* execute(RuntimeContext* context_i) = 0;
};


/*
 * Implementation of Print Statement
 */
class PrintStatement : public Statement {
    
public:
    PrintStatement(Expression* exp_i);
    ~PrintStatement();
    SymbolInfo* execute(RuntimeContext* context_i);
    
private:
    PrintStatement(PrintStatement& stmnt_i) {}
    PrintStatement& operator=(const PrintStatement& stmnt_i) {return *this;}
    
private:
    Expression* pmyExp;
};


/*
 * Implementation of  PrintLine Statement
 */
class PrintLineStatement : public Statement {
    
public:
    PrintLineStatement(Expression* exp_i);
    ~PrintLineStatement();
    SymbolInfo* execute(RuntimeContext* context_i);
    
private:
    PrintLineStatement(PrintLineStatement& stmnt_i) {}
    PrintLineStatement& operator=(const PrintLineStatement& stmnt_i) {return *this;}
    
private:
    Expression* pmyExp;
    
};


/*
 * Class to execute and evaluate an assignment operator
 */
class AssignmentStatement : public Statement {
   
public:
    AssignmentStatement(Variable* var_i, Expression* exp_i);
    AssignmentStatement(SymbolInfo* info_i, Expression* exp_i);
    ~AssignmentStatement();
    SymbolInfo* execute(RuntimeContext* context_i);
    
private:
    AssignmentStatement(AssignmentStatement& stmnt_i) {}
    AssignmentStatement& operator=(const AssignmentStatement& stmnt_i) {return *this;}
    
private:
    Variable*   pmyVar;
    Expression* pmyExp;
};


/*
 * Class to execute and evaluate an variable declaration
 */
class VariableDeclStatement : public Statement {
    
public:
    VariableDeclStatement(SymbolInfo* symbolInfo_i);
    ~VariableDeclStatement();
    SymbolInfo* execute(RuntimeContext* context_i);
    
private:
    VariableDeclStatement(VariableDeclStatement& stmnt_i) {}
    VariableDeclStatement& operator=(const VariableDeclStatement& stmnt_i) {return *this;}
    
private:
    SymbolInfo* pmySymbolInfo;
    Variable*   pmyVar;
};


/*
 * Class to evaluate an IF condition
 */
class IfStatement : public Statement {
    
public:
    IfStatement(Expression* exp_i,
                std::vector<Statement*> ifStatements_i,
                std::vector<Statement*> elseStatements_i);
    ~IfStatement();
    SymbolInfo* execute(RuntimeContext* context_i);
    
private:
    IfStatement(IfStatement& stmnt_i) {}
    IfStatement& operator=(const IfStatement& stmnt_i) {return *this;}
    
private:
    Expression* pmyCondition;
    std::vector<Statement*> myStatements;
    std::vector<Statement*> myElsePart;
};


/*
 * Class to evaluate an WHILE condition
 */
class WhileStatement : public Statement {
    
public:
    WhileStatement(Expression* exp_i, std::vector<Statement*> statements_i);
    ~WhileStatement();
    SymbolInfo* execute(RuntimeContext* context_i);
    
private:
    WhileStatement(WhileStatement& stmnt_i) {}
    WhileStatement& operator=(const WhileStatement& stmnt_i) {return *this;}
    
private:
    Expression* pmyCondition;
    std::vector<Statement*> myStatements;
};
#endif /* AST_Statement_hpp */
