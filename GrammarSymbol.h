//
// Created by jens on 01/06/23.
//

#ifndef COMPILER_GRAMMARSYMBOL_H
#define COMPILER_GRAMMARSYMBOL_H


#include <ostream>
#include "Token.h"

class GrammarSymbol {
public:
    using SymbolType = enum {
        TERMINAL, NONTERMINAL, EPSILON, END_OF_FILE
    };
private:
    GrammarSymbol(const SymbolType &type, const std::string &nonTerminal, const Token::TokenType &terminal);

    SymbolType type;
    std::string nonTerminal;
    Token::TokenType terminal;
public:
    static GrammarSymbol createNonTerminal(const std::string& name);
    static GrammarSymbol createTerminal(const Token::TokenType &token);
    static GrammarSymbol epsilon();
    static GrammarSymbol eof();

    [[nodiscard]] SymbolType getType() const;
    [[nodiscard]] std::string getNonTerminal() const;
    [[nodiscard]] Token::TokenType getTerminal() const;


    [[nodiscard]] bool isTerminal() const;
    [[nodiscard]] bool isEOF() const;
    [[nodiscard]] bool isEpsilon() const;

    bool operator==(const GrammarSymbol &other) const;
    bool operator!=(const GrammarSymbol &other) const;
    friend std::ostream &operator<<(std::ostream &os, const GrammarSymbol &grammarSymbol);

};


template<>
struct std::hash<GrammarSymbol> {
    std::size_t operator()(const GrammarSymbol &s) const noexcept {
        std::size_t hashValue = 0;
        hashValue ^= std::hash<GrammarSymbol::SymbolType>{}(s.getType());
        hashValue ^= std::hash<std::string>{}(s.getNonTerminal());
        hashValue ^= std::hash<Token::TokenType>{}(s.getTerminal());
        return hashValue;
    }
};


#endif //COMPILER_GRAMMARSYMBOL_H
