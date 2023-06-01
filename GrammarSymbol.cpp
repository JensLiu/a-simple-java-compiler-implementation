//
// Created by jens on 01/06/23.
//

#include "GrammarSymbol.h"

GrammarSymbol::SymbolType GrammarSymbol::getType() const {
    return this->type;
}

bool GrammarSymbol::operator==(const GrammarSymbol &other) const {
    if (this->type != other.type) {
        return false;
    }
    if (this->type == TERMINAL) {
        return this->terminal == other.terminal;
    } else if (this->type == NONTERMINAL) {
        return this->nonTerminal == other.nonTerminal;  // std::string == overloaded
    } else if (this->type == EPSILON || this->type == END_OF_FILE) {
        return true;
    } else {
        return false;
    }
}

bool GrammarSymbol::isTerminal() const {
    return this->type == SymbolType::TERMINAL || this->type == SymbolType::END_OF_FILE ||
           this->type == SymbolType::EPSILON;
}

GrammarSymbol::GrammarSymbol(const GrammarSymbol::SymbolType &type, const std::string &nonTerminal,
                             const Token::TokenType &terminal)
        : type(type), nonTerminal(nonTerminal), terminal(terminal) {}

GrammarSymbol GrammarSymbol::createNonTerminal(const std::string &name) {
    return GrammarSymbol(SymbolType::NONTERMINAL, name, Token::TokenType::INVALID_TOKEN);
}

GrammarSymbol GrammarSymbol::createTerminal(const Token::TokenType &token) {
    return GrammarSymbol(SymbolType::TERMINAL, "", token);
}

GrammarSymbol GrammarSymbol::epsilon() {
    return GrammarSymbol(SymbolType::EPSILON, "", Token::TokenType::EPSILON);
}

GrammarSymbol GrammarSymbol::eof() {
    return GrammarSymbol(SymbolType::END_OF_FILE, "", Token::TokenType::END_OF_FILE);
}

std::string GrammarSymbol::getNonTerminal() const {
    return this->nonTerminal;
}

Token::TokenType GrammarSymbol::getTerminal() const {
    return this->terminal;
}

bool GrammarSymbol::isEOF() const {
    return this->type == SymbolType::END_OF_FILE;
}

bool GrammarSymbol::isEpsilon() const {
    return this->type == SymbolType::EPSILON;
}

std::ostream &operator<<(std::ostream &os, const GrammarSymbol &grammarSymbol) {
    if (grammarSymbol.isTerminal()) {
        os << Token::tokenTypeAsString(grammarSymbol.getTerminal());
    } else {
        os << grammarSymbol.getNonTerminal();
    }
    return os;
}


