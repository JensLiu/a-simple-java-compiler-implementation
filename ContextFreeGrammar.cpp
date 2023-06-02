//
// Created by jens on 01/06/23.
//

#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include "ContextFreeGrammar.h"

// -------------------- CHANGE DETECTOR DEF START --------------------
// It can detect multiple changes in a single loop and is used to detect changes in the FIRST and FOLLOW sets.
// please use DETECT_CHANGE_BEGIN and DETECT_CHANGE_END in pairs, otherwise the detector will not work properly.
//
//  use case/semantic: repeat until nothing changes
//  usage:
//  ```
//      INSTALL_CHANGE_DETECTOR(<type-of-detection>)
//          while (CHANGE_DETECTOR_LOOP_CONDITION) {
//              RESET_CHANGE_DETECTOR
//              ...
//              DETECT_CHANGE_BEGIN(<expr-that-gives-you-the-initial-value>)
//                  <code-that-might-change-the-initial-value>
//              DETECT_CHANGE_END(<expr-that-gives-you-the-current-value>)
//              ...
//          }
//      UNINSTALL_CHANGE_DETECTOR
//  ```

#define INSTALL_CHANGE_DETECTOR(type) {\
bool my_detector_bool = true;   \
type my_detector_prev_val;      \
type my_detector_current_val;

#define UNINSTALL_CHANGE_DETECTOR }

#define CHANGE_DETECTOR_LOOP_CONDITION my_detector_bool
#define RESET_CHANGE_DETECTOR my_detector_bool = false;

#define DETECT_CHANGE_BEGIN(var) my_detector_prev_val = var; {;
#define DETECT_CHANGE_END(var) \
my_detector_current_val = var; \
if (my_detector_prev_val != my_detector_current_val) { \
    my_detector_bool = true; \
}}

// -------------------- CHANGE DETECTOR DEF END --------------------

ContextFreeGrammar::ContextFreeGrammar(const std::vector<Production> &productions)
        : productions(productions) {
    for (const Production &p: productions) {
        this->nonTermimals.insert(p.head);
        for (const GrammarSymbol &s: p.body) {
            if (s.isTerminal()) {
                this->terminals.insert(s);
            }
        }
        this->terminals.insert(GrammarSymbol::eof());   // end marker is also in the terminals
    }
}

void ContextFreeGrammar::findFirstForNonTerminals() {
    if (this->status >= FIRST_COMPUTED) return;
    INSTALL_CHANGE_DETECTOR(unsigned)
        while (CHANGE_DETECTOR_LOOP_CONDITION) {    // repeat until no set grows in size
            RESET_CHANGE_DETECTOR
            for (const Production &p: productions) {
                const GrammarSymbol &firstSymbol = p.body[0];
                DETECT_CHANGE_BEGIN(FIRST[p.head].size())
                    if (firstSymbol.isTerminal()) FIRST[p.head].insert(firstSymbol);
                    else FIRST[p.head].insert(FIRST[firstSymbol].begin(), FIRST[firstSymbol].end());
                    for (int i = 1; i <= p.body.size(); i++) {
                        const GrammarSymbol &prevSymbol = p.body[i - 1];
                        if (prevSymbol.isTerminal() || FIRST[prevSymbol].count(GrammarSymbol::epsilon()) == 0) {
                            break;
                        }
                        // The previous symbol is non-terminal and the empty string belongs to its FIRST set
                        // by induction: all FIRST sets of previous symbols contains the empty string
                        FIRST[p.head].insert(FIRST[prevSymbol].begin(), FIRST[prevSymbol].end());
                    }
                DETECT_CHANGE_END(FIRST[p.head].size())
            }
        }
    UNINSTALL_CHANGE_DETECTOR
    if (this->status < FIRST_COMPUTED) this->status = FIRST_COMPUTED;
}

void ContextFreeGrammar::findFirstForProductions() {
    if (this->status >= FIRST_P_COMPUTED) return;
    if (this->status < FIRST_COMPUTED) findFirstForNonTerminals();

    for (const Production &p: productions) {
        const GrammarSymbol &firstSymbol = p.body[0];
        if (firstSymbol.isTerminal()) FIRST_P[p].insert(firstSymbol);
        else FIRST_P[p].insert(FIRST[firstSymbol].begin(), FIRST[firstSymbol].end());
        for (int i = 1; i <= p.body.size(); i++) {
            // FIRST sets of non-terminals are already computed therefore one scan for each production suffices
            const GrammarSymbol &prevSymbol = p.body[i - 1];
            if (prevSymbol.isTerminal() ||
                FIRST[prevSymbol].count(GrammarSymbol::epsilon()) == 0) {
                break;
            }
            // The previous symbol is non-terminal and the empty string belongs to its FIRST set
            FIRST_P[p].insert(FIRST[prevSymbol].begin(), FIRST[prevSymbol].end());
        }
    }

    if (this->status < FIRST_P_COMPUTED) this->status = FIRST_P_COMPUTED;
}

void ContextFreeGrammar::findFollow() {
    if (this->status >= FOLLOW_COMPUTED) return;
    if (this->status < FIRST_P_COMPUTED) findFirstForProductions();

    // the endmarker eof belongs to FIRST[start]
    FOLLOW[productions[0].head].insert(GrammarSymbol::eof());
    INSTALL_CHANGE_DETECTOR(unsigned)
        while (CHANGE_DETECTOR_LOOP_CONDITION) {    // repeat until no set grows in size
            RESET_CHANGE_DETECTOR
            for (const Production &p: productions) {
                bool nullableSuffix = true;     // the last symbol in the body always has a nullable suffix
                for (int i = p.body.size() - 1; i >= 0; i--) {  // backward scan of the body
                    const GrammarSymbol &current = p.body[i];
                    if (nullableSuffix && !current.isTerminal()) {
                        // if the current symbol has a nullable suffix, FOLLOW[head] is a subset of FOLLOW[current]
                        DETECT_CHANGE_BEGIN(FOLLOW[current].size())
                            FOLLOW[current].insert(FOLLOW[p.head].begin(),
                                                   FOLLOW[p.head].end());   // head is guaranteed to be non-terminal by CFG
                            FOLLOW[current].erase(GrammarSymbol::epsilon());
                        DETECT_CHANGE_END(FOLLOW[current].size())
                    }
                    if (i - 1 >= 0 && !p.body[i - 1].isTerminal()) {
                        // if the symbol right to the current symbol exists and is non-terminal,
                        // FIRST[current] is a subset of FOLLOW[current - 1]
                        DETECT_CHANGE_BEGIN(FOLLOW[p.body[i - 1]].size())
                            if (current.isTerminal()) FOLLOW[p.body[i - 1]].insert(current);
                            else FOLLOW[p.body[i - 1]].insert(FIRST[current].begin(), FIRST[current].end());
                            FOLLOW[p.body[i - 1]].erase(GrammarSymbol::epsilon());
                        DETECT_CHANGE_END(FOLLOW[p.body[i - 1]].size())
                    }
                    // update if the suffix containing current symbol is nullable
                    if (current.isTerminal() || FIRST[current].count(GrammarSymbol::epsilon()) == 0) {
                        nullableSuffix = false;
                    }
                }
            }
        }
    UNINSTALL_CHANGE_DETECTOR

    if (this->status < FOLLOW_COMPUTED) this->status = FOLLOW_COMPUTED;
}

void ContextFreeGrammar::findParsingTableLL1() {
    if (this->status >= PARSING_TABLE_COMPUTED) return;
    if (this->status < FOLLOW_COMPUTED) findFollow();

    for (const GrammarSymbol &s: nonTermimals) {
        // find all productions of this symbol

        parsing_table_entry_t entryForS;

        for (const Production &p: productions) {
            if (p.head != s) continue;
            // for each production with head s

            // NOTE: Had there been any collision when
            // (1) epsilon is in FIRST[s] and
            // (2) FIRST[s] intersection FOLLOW[s] is not empty,
            // place the production of preference below so that it can override the previous effect

            if (p.isEpsilon()) {
                // for productions such as A ::= epsilon, we add it to columns with respect to their FOLLOW set
                for (const GrammarSymbol &nt: FOLLOW[s]) {
                    entryForS.insert({nt, p});
                }
            } else {
                for (const GrammarSymbol &nt: FIRST_P[p]) {  // for each lookahead of this production, assuming LL(1)
                    entryForS.insert({nt, p});   // add to the appropriate column: entry[terminal] = production
                }
            }
        }

        PARSING_TABLE.insert({s, entryForS});   // set entry to the corresponding head: PARSING_TABLE[non-terminal] = entry
    }
    if (this->status < PARSING_TABLE_COMPUTED) this->status = PARSING_TABLE_COMPUTED;
}

void ContextFreeGrammar::printParsingTable() {
    if (this->status < PARSING_TABLE_COMPUTED) findParsingTableLL1();

    for (const GrammarSymbol &nt: nonTermimals) {
        std::cout << nt << std::endl;
        for (const GrammarSymbol &t: terminals) {
            auto it = PARSING_TABLE[nt].find(t);
            if (it == PARSING_TABLE[nt].end()) {
                std::cout << "\ton " << t << "\terror" << std::endl;
            } else {
                std::cout << "\ton "  << t << "\tuse " << it->second << std::endl;
            }
        }
        std::cout << std::endl;
    }
}

void ContextFreeGrammar::printFirstTableForNonTerminals() {
    if (status < FIRST_COMPUTED) findFirstForNonTerminals();
    for (const auto &pair: FIRST) {
        std::cout << "FIRST[" << pair.first << "] = {";
        for (const GrammarSymbol &symbol: pair.second) {
            std::cout << "'" << symbol << "' ";
        }
        std::cout << "}" << std::endl;
    }
}

void ContextFreeGrammar::printFirstTableForProductions() {
    if (status < FIRST_P_COMPUTED) findFirstForProductions();
    for (const auto &pair: FIRST_P) {
        std::cout << "FIRST_P[" << pair.first << "] = {";
        for (const GrammarSymbol &symbol: pair.second) {
            std::cout << "'" << symbol << "' ";
        }
        std::cout << "}" << std::endl;
    }
}

void ContextFreeGrammar::printFollowTable() {
    if (status < FOLLOW_COMPUTED) findFollow();
    for (const auto &pair: FOLLOW) {
        std::cout << "FOLLOW[" << pair.first << "] = {";
        for (const GrammarSymbol &symbol: pair.second) {
            std::cout << "'" << symbol << "' ";
        }
        std::cout << "}" << std::endl;
    }
}

GrammarSymbol &ContextFreeGrammar::getStartSymbol() {
    return productions[0].head;
}

std::variant<Production, ErrorStrategy>
ContextFreeGrammar::predict(const GrammarSymbol &current, const GrammarSymbol &onInput) {
    if (this->status < PARSING_TABLE_COMPUTED) findParsingTableLL1();

    const Production &p = PARSING_TABLE[current][onInput];
    if (!p.isValid()) {
        return ErrorStrategy("error, do nothing");
    }
    return p;
}
