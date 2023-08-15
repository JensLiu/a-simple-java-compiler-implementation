//
// Created by jens on 01/06/23.
//

#include <iostream>
#include "Production.h"

int Production::nextId = 0;

std::ostream &operator<<(std::ostream &os, const Production &production) {
    os << production.head << " ::= ";
    for (const GrammarSymbol &symbol: production.body) {
        os << symbol << " ";
    }
    return os;
}

Production::Production() : head(GrammarSymbol::invalid()), id(-1) {}

Production::Production(const GrammarSymbol &head, const std::vector<GrammarSymbol> &body)
        : head(head), body(body) {
    if (this->body.size() == 0) {
        this->body.push_back(GrammarSymbol::epsilon());
    }
    this->id = nextId;
    nextId++;
}

Production::Production(int id, const GrammarSymbol &head, const std::vector<GrammarSymbol> &body)
        : id(id), head(head), body(body) {
    if (this->body.size() == 0) {
        this->body.push_back(GrammarSymbol::epsilon());
    }
    nextId = id + 1;
}

bool Production::operator==(const Production &other) const {
    return this->id == other.id;
//    if (this->head != other.head) {
//        return false;
//    }
//    if (this->body.size() != other.body.size()) {
//        return false;
//    }
//    for (int i = 0; i < this->body.size(); i++) {
//        if (this->body[i] != other.body[i]) {
//            return false;
//        }
//    }
//    return true;
}

bool Production::isEpsilonProduction() const {
    if (this->body.size() > 1) {
        return false;
    }
    return this->body[0] == GrammarSymbol::epsilon();
}

bool Production::isValid() const {
    return this->id != -1;
}

int Production::getId() const {
    return this->id;
}
