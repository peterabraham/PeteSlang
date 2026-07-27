//
//  SymbolTable.h
//  PeteSlang
//
//  Created by Peter on 10/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#pragma once

#include <map>
#include <string>

// Forward declaration
class SymbolInfo;
class Variable;

/*
 * Symbol Table for Parsing and Type Analysis.
 *
 * The table is a lookup index, not an owner: SymbolInfo objects are
 * owned by the AST node that declared them (VariableDeclStatement),
 * which outlives both the CompilationContext's and the
 * RuntimeContext's tables that reference it. The table therefore
 * stores non-owning observer pointers, same as the original design,
 * just made explicit instead of accidental.
 */
class SymbolTable {

public:
    SymbolTable();
    bool add(SymbolInfo* info_i);
    void assign(std::string varName_i, SymbolInfo* info_i);
    void assign(Variable* var_i, SymbolInfo* info_i);
    SymbolInfo* get(std::string varName_i);
private:
    std::map<std::string, SymbolInfo*> myTable;
};
