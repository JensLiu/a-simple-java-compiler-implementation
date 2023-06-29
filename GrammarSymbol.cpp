//
// Created by jens on 01/06/23.
//

#include "GrammarSymbol.h"

GrammarSymbol::SymbolType GrammarSymbol::getType() const {
    return this->type;
}

bool GrammarSymbol::operator==(const GrammarSymbol &other) const {
    if (this->type == INVALID_SYMBOL) {
        return false;
    }
    if (this->type != other.type) {
        return false;
    }
    if (this->type == TERMINAL) {
        return this->terminal == other.terminal;
    } else if (this->type == NONTERMINAL) {
        return this->nonTerminal == other.nonTerminal;  // std::string == overloaded
    } else {
        return false;
    }
}

bool GrammarSymbol::isTerminal() const {
    return this->type == SymbolType::TERMINAL;
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
    return GrammarSymbol(SymbolType::TERMINAL, "", Token::TokenType::EPSILON);
}

GrammarSymbol GrammarSymbol::eof() {
    return GrammarSymbol(SymbolType::TERMINAL, "", Token::TokenType::END_OF_FILE);
}

std::string GrammarSymbol::getNonTerminal() const {
    return this->nonTerminal;
}

Token::TokenType GrammarSymbol::getTerminal() const {
    return this->terminal;
}

bool GrammarSymbol::isEOF() const {
    return this->terminal == Token::TokenType::END_OF_FILE;
}

bool GrammarSymbol::isEpsilon() const {
    return this->terminal == Token::TokenType::EPSILON;
}

std::ostream &operator<<(std::ostream &os, const GrammarSymbol &grammarSymbol) {
    if (grammarSymbol.isTerminal()) {
        os << Token::tokenTypeAsString(grammarSymbol.getTerminal());
    } else {
        os << grammarSymbol.getNonTerminal();
    }
    return os;
}

bool GrammarSymbol::operator!=(const GrammarSymbol &other) const {
    return !(*this == other);
}

GrammarSymbol GrammarSymbol::invalid() {
    return GrammarSymbol(SymbolType::INVALID_SYMBOL, "", Token::TokenType::INVALID_TOKEN);
}

bool GrammarSymbol::isValid() const {
    return this->type != INVALID_SYMBOL;
}

GrammarSymbol::GrammarSymbol() {
    this->type = INVALID_SYMBOL;
    this->terminal = Token::TokenType::INVALID_TOKEN;
    this->nonTerminal = "";
}