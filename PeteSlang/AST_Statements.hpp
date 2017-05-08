//
//  AST_Statements.hpp
//  PeteSlang
//
//  Created by Peter on 05/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#ifndef AST_Statements_hpp
#define AST_Statements_hpp

#include "AST.hpp"

/*
 * Statement is what you Execute for it's Effect
 */
class Statement {
    
public:
    virtual ~Statement() = 0;
    virtual bool execute(const RuntimeContext* context_i) = 0;
};


/*
 * Implementation of Print Statement
 */
class PrintStatement : public Statement {
    
public:
    PrintStatement(Expression* exp_i);
    ~PrintStatement();
    bool execute(const RuntimeContext* context_i);
    
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
    bool execute(const RuntimeContext* context_i);
    
private:
    PrintLineStatement(PrintLineStatement& stmnt_i) {}
    PrintLineStatement& operator=(const PrintLineStatement& stmnt_i) {return *this;}
    
private:
    Expression* pmyExp;
    
};
#endif /* AST_Statement_hpp */
