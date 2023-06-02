//
// Created by jens on 01/06/23.
//

#ifndef COMPILER_CONTEXTFREEGRAMMAR_H
#define COMPILER_CONTEXTFREEGRAMMAR_H


#include <unordered_set>
#include <variant>
#include "GrammarSymbol.h"
#include "Production.h"

class ErrorStrategy {
private:
    std::string message;
public:
    ErrorStrategy(const std::string &message): message(message) {}
};

class ContextFreeGrammar {
public:
    using first_set_entry_t = std::unordered_set<GrammarSymbol>;
    using first_set_terminal_t = std::unordered_map<GrammarSymbol, first_set_entry_t>;
    using follow_set_entry_t = std::unordered_set<GrammarSymbol>;
    using follow_set_t = std::unordered_map<GrammarSymbol, follow_set_entry_t>;
    using first_set_production_t = std::unordered_map<Production, first_set_entry_t>;
    using parsing_table_entry_t = std::unordered_map<GrammarSymbol, Production>;
    using parsing_table_t = std::unordered_map<GrammarSymbol, parsing_table_entry_t>;

private:
    std::unordered_set<GrammarSymbol> nonTermimals;
    std::unordered_set<GrammarSymbol> terminals;
    std::vector<Production> productions;
    using InternalStatus = enum {INITIAL, FIRST_COMPUTED, FIRST_P_COMPUTED, FOLLOW_COMPUTED, PARSING_TABLE_COMPUTED};
    InternalStatus status = INITIAL;
    first_set_terminal_t FIRST;
    first_set_production_t FIRST_P;
    follow_set_t FOLLOW;
    parsing_table_t PARSING_TABLE;


public:
    explicit ContextFreeGrammar(const std::vector<Production> &productions);
    void findFirstForNonTerminals();
    void findFirstForProductions();
    void findFollow();
    void findParsingTableLL1();
    void printFirstSetForNonTerminals();
    void printFirstSetForProductions();
    void printFollowSet();
    void printParsingTable();
    void printProductions();

    GrammarSymbol &getStartSymbol();
    std::variant<Production, ErrorStrategy> predict(const GrammarSymbol &current, const GrammarSymbol &onInput);
};

#endif //COMPILER_CONTEXTFREEGRAMMAR_H
