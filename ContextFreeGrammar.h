//
// Created by jens on 01/06/23.
//

#ifndef COMPILER_CONTEXTFREEGRAMMAR_H
#define COMPILER_CONTEXTFREEGRAMMAR_H


#include <unordered_set>
#include "GrammarSymbol.h"
#include "Production.h"

class ContextFreeGrammar {
public:
    using first_set_entry_t = std::unordered_set<GrammarSymbol>;
    using first_set_t = std::unordered_map<GrammarSymbol, first_set_entry_t>;
    using follow_set_entry_t = std::unordered_set<GrammarSymbol>;
    using follow_set_t = std::unordered_map<GrammarSymbol, follow_set_entry_t>;
private:
    std::unordered_set<GrammarSymbol> nonTermimals;
    std::vector<Production> productions;
    first_set_t FIRST;
    follow_set_t FOLLOW;
    void initFirstAndFollowSet();

public:
    ContextFreeGrammar(const std::vector<Production> &productions);

    void findFirstForNonTerminals();
};


#endif //COMPILER_CONTEXTFREEGRAMMAR_H
