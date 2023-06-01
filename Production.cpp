//
// Created by jens on 01/06/23.
//

#include <iostream>
#include "Production.h"

int Production::nextId = 0;

std::ostream &operator<<(std::ostream &os, const Production &production) {
    os << production.head << " -> ";
    for (const GrammarSymbol &symbol: production.body) {
        os << symbol << " ";
    }
    return os;
}

void Production::printProduction() const {
    std::cout << this->head.getNonTerminal() << " -> ";
    for (const GrammarSymbol &symbol: this->body) {
        if (symbol.isTerminal()) {
            std::cout << symbol.getTerminal() << " ";
        } else {
            std::cout << symbol.getNonTerminal() << " ";
        }
    }
    std::cout << std::endl;
}

Production::Production(const GrammarSymbol &head, const std::vector<GrammarSymbol> &body)
        : head(head), body(body) {
    this->id = nextId;
    nextId++;
}

Production::Production(int id, const GrammarSymbol &head, const std::vector<GrammarSymbol> &body)
        : id(id), head(head), body(body) { nextId = id + 1; }