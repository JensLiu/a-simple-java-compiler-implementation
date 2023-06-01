//
// Created by jens on 01/06/23.
//

#include <unordered_map>
#include <iostream>
#include "ContextFreeGrammar.h"

// -------------------- CHANGE DETECTOR DEF START --------------------
// It can detect multiple changes in a single loop and is used to detect changes in the FIRST and FOLLOW sets.
// please use DETECT_CHANGE_START and DETECT_CHANGE_END in pairs, otherwise the detector will not work properly.

#define INSTALL_CHANGE_DETECTOR(type) {\
bool my_detector_bool = true;   \
type my_detector_prev_val = 0;      \
type my_detector_current_val = 0;

#define CHANGE_DETECTOR_LOOP_CONDITION my_detector_bool
#define RESET_CHANGE_DETECTOR my_detector_bool = false;

#define DETECT_CHANGE_START(var) my_detector_prev_val = var; {;

#define DETECT_CHANGE_END(var) \
my_detector_current_val = var; \
if (my_detector_prev_val != my_detector_current_val) { \
    my_detector_bool = true; \
}}

#define UNINSTALL_CHANGE_DETECTOR }

// -------------------- CHANGE DETECTOR DEF END --------------------

ContextFreeGrammar::ContextFreeGrammar(const std::vector<Production> &productions)
        : productions(productions) {
    for (const Production &production: productions) {
        this->nonTermimals.insert(production.head);
    }
    initFirstAndFollowSet();
}


void ContextFreeGrammar::findFirstForNonTerminals() {
    INSTALL_CHANGE_DETECTOR(unsigned)
        while (CHANGE_DETECTOR_LOOP_CONDITION) {
            RESET_CHANGE_DETECTOR
            for (const Production &p: productions) {
                const GrammarSymbol &firstSymbol = p.body[0];
                DETECT_CHANGE_START(FIRST[p.head].size())
                    if (firstSymbol.isTerminal()) FIRST[p.head].insert(firstSymbol);
                    else FIRST[p.head].insert(FIRST[firstSymbol].begin(), FIRST[firstSymbol].end());
                    for (int i = 1; i <= p.body.size(); i++) {
                        const GrammarSymbol &prevSymbol = p.body[i - 1];
                        if (prevSymbol.isTerminal() ||
                            FIRST[prevSymbol].find(GrammarSymbol::epsilon()) == FIRST[prevSymbol].end()) {
                            break;
                        }
                        // !prevSymbol.isTerminal() && FIRST[prevSymbol].find(GrammarSymbol::epsilon()) != FIRST[prevSymbol].end()
                        FIRST[p.head].insert(FIRST[prevSymbol].begin(), FIRST[prevSymbol].end());
                    }
                DETECT_CHANGE_END(FIRST[p.head].size())
            }
        }
    UNINSTALL_CHANGE_DETECTOR
}

void ContextFreeGrammar::initFirstAndFollowSet() {
    for (const GrammarSymbol &nt: nonTermimals) {
        FIRST.insert({nt, first_set_entry_t{}});
        FOLLOW.insert({nt, follow_set_entry_t{}});
    }
}
