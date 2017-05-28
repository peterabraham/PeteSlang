//
//  SymbolTable.cpp
//  PeteSlang
//
//  Created by Peter on 10/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#include "SymbolTable.hpp"
#include "AST_Statements.hpp"

using namespace std;


/*
 * Ctor of the class SymbolTable
 */
SymbolTable::SymbolTable() {
    
}


/*
 * Function to add a symbol to the symbol table
 */
bool SymbolTable::add(SymbolInfo* info_i) {
    myTable.insert(make_pair(info_i->mySymbolName, info_i));
    return true;
}


/*
 * Function to assign symbol info to an exiting variable in symbol table
 */
void SymbolTable::assign(std::string varName_i, SymbolInfo* info_i) {
    myTable.insert(make_pair(varName_i, info_i));
}


/*
 * Function to assign symbol info to an exiting variable in symbol table
 */
void SymbolTable::assign(Variable* var_i, SymbolInfo* info_i) {
    info_i->mySymbolName = var_i->getName();
    map<std::string, SymbolInfo*> ::iterator it = myTable.find(info_i->mySymbolName);
    if (it != myTable.end()) {
        it->second = info_i;
    } else {
        myTable.insert(make_pair(info_i->mySymbolName, info_i));
    }
}


/*
 * Function to get the Symbol info of an
 * existing variable from the symbol table
 */
SymbolInfo* SymbolTable::get(std::string varName_i) {
    map<string, SymbolInfo*>::iterator it;
    it = myTable.find(varName_i);
    return it->second;
}
