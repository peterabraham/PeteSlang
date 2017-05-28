//
//  RuntimeContext.hpp
//  PeteSlang
//
//  Created by Peter on 29/04/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#ifndef MemoryContext_hpp
#define MemoryContext_hpp

#include "SymbolTable.hpp"

/*
 * A Context is necessary for Variable scope
 */
class RuntimeContext {
    
public:
    RuntimeContext();
    ~RuntimeContext();
    SymbolTable* getSymbolTable();
    SymbolInfo* getInfo(std::string varName_i);
    void addInfo(SymbolInfo* info_i);
    void assignInfo(Variable* var_i, SymbolInfo* info_i);
    void setSymbolTable(SymbolTable* table_i);
    
private:
    SymbolTable* mySymbolTable;
};

/*
 * A Context is necessary for Variable scope
 */
class CompilationContext {
public:
    CompilationContext();
    ~CompilationContext();
    SymbolTable* getSymbolTable();
    SymbolInfo* getInfo(std::string varName_i);
    void addInfo(SymbolInfo* info_i);
    void assignInfo(Variable* var_i, SymbolInfo* info_i);
    void setSymbolTable(SymbolTable* table_i);
    
private:
    SymbolTable* mySymbolTable;
};

#endif /* MemoryContext_hpp */
