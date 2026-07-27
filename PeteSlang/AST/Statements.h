//
//  Statements.h
//  PeteSlang
//
//  Created by Peter on 05/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#pragma once

#include "AST/Expressions.h"
#include <memory>
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
    PrintStatement(std::unique_ptr<Expression> exp_i);
    SymbolInfo* execute(RuntimeContext* context_i) override;

private:
    std::unique_ptr<Expression> pmyExp;
};


/*
 * Implementation of  PrintLine Statement
 */
class PrintLineStatement : public Statement {

public:
    PrintLineStatement(std::unique_ptr<Expression> exp_i);
    SymbolInfo* execute(RuntimeContext* context_i) override;

private:
    std::unique_ptr<Expression> pmyExp;

};


/*
 * Class to execute and evaluate an assignment operator.
 *
 * pmyVar is always owned here (std::unique_ptr<Variable>): either
 * moved in directly, or constructed internally from a SymbolInfo*.
 * The original code allocated a Variable in the SymbolInfo* ctor and
 * left the delete commented out in the dtor (a real leak) -
 * unique_ptr fixes that for free.
 */
class AssignmentStatement : public Statement {

public:
    AssignmentStatement(std::unique_ptr<Variable> var_i, std::unique_ptr<Expression> exp_i);
    AssignmentStatement(SymbolInfo* info_i, std::unique_ptr<Expression> exp_i);
    SymbolInfo* execute(RuntimeContext* context_i) override;

private:
    std::unique_ptr<Variable>   pmyVar;
    std::unique_ptr<Expression> pmyExp;
};


/*
 * Class to execute and evaluate an variable declaration.
 *
 * pmySymbolInfo is the single true owner of the declared variable's
 * SymbolInfo (see Runtime/SymbolTable.h for why the tables themselves
 * only hold non-owning observer pointers into it).
 */
class VariableDeclStatement : public Statement {

public:
    VariableDeclStatement(std::unique_ptr<SymbolInfo> symbolInfo_i);
    SymbolInfo* execute(RuntimeContext* context_i) override;

private:
    std::unique_ptr<SymbolInfo> pmySymbolInfo;
    std::unique_ptr<Variable>   pmyVar;
};


/*
 * Class to evaluate an IF condition
 */
class IfStatement : public Statement {

public:
    IfStatement(std::unique_ptr<Expression> exp_i,
                std::vector<std::unique_ptr<Statement>> ifStatements_i,
                std::vector<std::unique_ptr<Statement>> elseStatements_i);
    SymbolInfo* execute(RuntimeContext* context_i) override;

private:
    std::unique_ptr<Expression> pmyCondition;
    std::vector<std::unique_ptr<Statement>> myStatements;
    std::vector<std::unique_ptr<Statement>> myElsePart;
};


/*
 * Class to evaluate an WHILE condition
 */
class WhileStatement : public Statement {

public:
    WhileStatement(std::unique_ptr<Expression> exp_i, std::vector<std::unique_ptr<Statement>> statements_i);
    SymbolInfo* execute(RuntimeContext* context_i) override;

private:
    std::unique_ptr<Expression> pmyCondition;
    std::vector<std::unique_ptr<Statement>> myStatements;
};
