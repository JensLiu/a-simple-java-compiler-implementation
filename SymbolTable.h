//
// Created by jens on 02/06/23.
//

#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H


#include <string>
#include <vector>

class SymbolTableEntry {
// for now...
public:
    std::string name;
    explicit SymbolTableEntry(const std::string &name);
};

class SymbolTable {
private:
    std::vector<SymbolTableEntry> table;
public:
    int addSymbol(const std::string &symbol);
//    int add(const SymbolTableEntry &entry);
    SymbolTableEntry &get(const int &index);
    std::string &getSymbol(const int &index);
};

#endif //COMPILER_SYMBOLTABLE_H
