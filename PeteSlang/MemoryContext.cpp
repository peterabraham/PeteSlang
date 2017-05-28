//
//  RuntimeContext.cpp
//  PeteSlang
//
//  Created by Peter on 29/04/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#include "MemoryContext.hpp"
#include "Common.hpp"

/*
 * Ctor of the class RuntimeContext
 */
RuntimeContext::RuntimeContext() : mySymbolTable(nullptr){
    mySymbolTable = new SymbolTable();
}


/*
 * Dtor of the class RuntimeContext
 */
RuntimeContext::~RuntimeContext() {
    safe_delete(mySymbolTable);
}


/*
 * Function to retrieve the symbol table
 */
SymbolTable* RuntimeContext::getSymbolTable() {
    return mySymbolTable;
}


/*
 * Function to set the symbol table.
 */
void RuntimeContext::setSymbolTable(SymbolTable* table_i) {
    mySymbolTable = table_i;
}


/*
 * Function to retrieve the symbol info 
 * of input argument variable.
 */
SymbolInfo* RuntimeContext::getInfo(std::string varName_i) {
    return mySymbolTable->get(varName_i);
}


/*
 * Function to add symbol info to symbol table
 */
void RuntimeContext::addInfo(SymbolInfo* info_i) {
    mySymbolTable->add(info_i);
}


/*
 * Function to assign symbol info to an 
 * existing variable in symbol table
 */
void RuntimeContext::assignInfo(Variable* var_i, SymbolInfo* info_i) {
    mySymbolTable->assign(var_i, info_i);
}


/*
 * Ctor of the class CompilationContext
 */
CompilationContext::CompilationContext() {
    mySymbolTable = new SymbolTable();
}


/*
 * Dtor of the class CompilationContext
 */
CompilationContext::~CompilationContext() {
    safe_delete(mySymbolTable);
}


/*
 * Function to retrieve the symbol table
 */
SymbolTable* CompilationContext::getSymbolTable() {
    return mySymbolTable;
}


/*
 * Function to retrieve the symbol 
 * info of input argument variable.
 */
SymbolInfo* CompilationContext::getInfo(std::string varName_i) {
    return mySymbolTable->get(varName_i);
}


/*
 * Function to set the symbol table.
 */
void CompilationContext::setSymbolTable(SymbolTable* table_i) {
    mySymbolTable = table_i;
}


/*
 * Function to add symbol info to symbol table
 */
void CompilationContext::addInfo(SymbolInfo* info_i) {
    mySymbolTable->add(info_i);
}


/*
 * Function to assign symbol info to an
 * existing variable in symbol table
 */
void CompilationContext::assignInfo(Variable* var_i, SymbolInfo* info_i) {
    mySymbolTable->assign(var_i, info_i);
}
