//
// Created by jens on 02/06/23.
//

#include "SymbolTable.h"

//int SymbolTable::add(const SymbolTableEntry &entry) {
//    table.push_back(entry);
//    return table.size() - 1;
//}

SymbolTableEntry &SymbolTable::get(const int &index) {
    return table.at(index);
}

int SymbolTable::addSymbol(const std::string &symbol) {
    int i = 0;
    // this function is O(|table| * |symbol|)
    // for each adding of identifier
    for (; i < table.size(); i++) {
        if (table.at(i).name == symbol)
            return i;
    }
    table.emplace_back(symbol);
    // otherwise add
    return table.size() - 1;
}

std::string &SymbolTable::getSymbol(const int &index) {
    return table.at(index).name;
}

SymbolTableEntry::SymbolTableEntry(const std::string &name) : name(name) {

}
