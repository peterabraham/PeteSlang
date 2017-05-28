//
//  SymbolTable.hpp
//  PeteSlang
//
//  Created by Peter on 10/05/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#ifndef SymbolTable_hpp
#define SymbolTable_hpp

#include <map>

// Forward declaration
class SymbolInfo;
class Variable;

/*
 *  Symbol Table for Parsing and Type Analysis
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
#endif /* SymbolTable_hpp */
