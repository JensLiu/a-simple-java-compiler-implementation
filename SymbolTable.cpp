//
// Created by jens on 02/06/23.
//

#include "SymbolTable.h"

int SymbolTable::add(const SymbolTableEntry &entry) {
    table.push_back(entry);
    return table.size() - 1;
}

SymbolTableEntry &SymbolTable::get(const int &index) {
    return table.at(index);
}

SymbolTableEntry::SymbolTableEntry(const std::string &name) : name(name) {}
