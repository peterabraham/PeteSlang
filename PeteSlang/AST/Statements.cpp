//
//  Statements.cpp
//  PeteSlang
//
//  Created by Peter on 05/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#include "AST/Statements.h"
#include <utility>

/*
 * Dtor of Statement class
 */
Statement::~Statement() {
}


/*
 * Ctor of PrintStatement class
 */
PrintStatement::PrintStatement(std::unique_ptr<Expression> exp_i) : pmyExp(std::move(exp_i)) {
}


/*
 * This function evaluates the expression and
 * spits the value to the console using
 * std::cout statement.
 */
SymbolInfo* PrintStatement::execute(RuntimeContext* context_i) {
    SymbolInfo* retVal = pmyExp->evaluate(context_i);

    switch (retVal->myType) {
        case TypeInfo::TYPE_NUMERIC:
            std::cout << retVal->getDouble();
            break;

        case TypeInfo::TYPE_STRING:
            std::cout << retVal->getString();
            break;

        case TypeInfo::TYPE_BOOL:
            std::cout << (retVal->getBool() ? "TRUE" : "FALSE");
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
PrintLineStatement::PrintLineStatement(std::unique_ptr<Expression> exp_i) : pmyExp(std::move(exp_i)) {
}


/*
 * This print function appends std::endl with
 * std::cout to emit an extra new line.
 */
SymbolInfo* PrintLineStatement::execute(RuntimeContext* context_i) {
    SymbolInfo* retVal = pmyExp->evaluate(context_i);

    switch (retVal->myType) {
        case TypeInfo::TYPE_NUMERIC:
            std::cout << retVal->getDouble();
            break;

        case TypeInfo::TYPE_STRING:
            std::cout << retVal->getString();
            break;

        case TypeInfo::TYPE_BOOL:
            std::cout << (retVal->getBool() ? "TRUE" : "FALSE");
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
AssignmentStatement::AssignmentStatement(std::unique_ptr<Variable> var_i, std::unique_ptr<Expression> exp_i)
    : pmyVar(std::move(var_i)), pmyExp(std::move(exp_i)) {
}


/*
 * Ctor of AssignmentStatement class
 */
AssignmentStatement::AssignmentStatement(SymbolInfo* info_i, std::unique_ptr<Expression> exp_i) {
    pmyVar = std::make_unique<Variable>(info_i);
    pmyExp = std::move(exp_i);
}


/*
 * Function to evaluate and execute assignment operator
 */
SymbolInfo* AssignmentStatement::execute(RuntimeContext* context_i) {
    SymbolInfo* value = pmyExp->evaluate(context_i);
    context_i->assignInfo(pmyVar.get(), value);

    return nullptr;
}


///-------------------------------------------------------------------
/*
 * Ctor of VariableDeclStatement class
 */
VariableDeclStatement::VariableDeclStatement(std::unique_ptr<SymbolInfo> symbolInfo_i) : pmySymbolInfo(std::move(symbolInfo_i)) {
}


/*
 * Function to evaluate and execute variable declaration
 */
SymbolInfo* VariableDeclStatement::execute(RuntimeContext* context_i) {
    context_i->addInfo(pmySymbolInfo.get());
    pmyVar = std::make_unique<Variable>(pmySymbolInfo.get());

    return nullptr;
}


///-------------------------------------------------------------------
/*
 * Ctor of IfStatement class
 */
IfStatement::IfStatement(std::unique_ptr<Expression> exp_i,
                         std::vector<std::unique_ptr<Statement>> ifStatements_i,
                         std::vector<std::unique_ptr<Statement>> elseStatements_i) : pmyCondition(std::move(exp_i)),
                                                                     myStatements(std::move(ifStatements_i)),
                                                                     myElsePart(std::move(elseStatements_i)) {
}


/*
 * Function to evaluate the if statement.
 */
SymbolInfo* IfStatement::execute(RuntimeContext* context_i) {
    SymbolInfo* pRetSymbol = pmyCondition->evaluate(context_i);

    if (nullptr == pRetSymbol && TypeInfo::TYPE_BOOL != pRetSymbol->myType) {
        return nullptr;
    }

    if (true == pRetSymbol->getBool()) {
        for (auto& st : myStatements) {
            st->execute(context_i);
        }
    } else if (false  == pRetSymbol->getBool()) {
        for (auto& st : myElsePart) {
            st->execute(context_i);
        }
    }

    // NOTE: this clears myStatements twice (pre-existing behavior from
    // before the unique_ptr conversion, most likely a copy/paste slip
    // that should have cleared myElsePart the second time). Left as-is
    // to keep output byte-identical to the original; worth revisiting
    // once IfStatement can be re-entered from an enclosing loop.
    myStatements.clear();
    myStatements.clear();

    return nullptr;
}


///-------------------------------------------------------------------
/*
 * Ctor of WhileStatement class
 */
WhileStatement::WhileStatement(std::unique_ptr<Expression> exp_i,
                               std::vector<std::unique_ptr<Statement>> statements_i) : pmyCondition(std::move(exp_i)),
                                                                       myStatements(std::move(statements_i)) {
}


/*
 * Function to execute the while statement
 */
SymbolInfo* WhileStatement::execute(RuntimeContext* context_i) {
Loop:
    SymbolInfo* pInfo = pmyCondition->evaluate(context_i);

    if (nullptr == pInfo && TypeInfo::TYPE_BOOL != pInfo->myType) {
        myStatements.clear();
        return nullptr;
    }

    if (true != pInfo->getBool()) {
        myStatements.clear();
        return nullptr;
    }

    SymbolInfo* pRetSymbol = nullptr;
    for (auto& st : myStatements) {
        pRetSymbol = st->execute(context_i);
        if (nullptr != pRetSymbol) {
            myStatements.clear();
            return pRetSymbol;
        }
    }

    goto Loop;
}
