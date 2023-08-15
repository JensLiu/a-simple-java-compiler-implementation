//
// Created by jens on 01/06/23.
//

#ifndef COMPILER_PRODUCTION_H
#define COMPILER_PRODUCTION_H

#include <vector>
#include "GrammarSymbol.h"

class Production {
private:
    int id;
    static int nextId;
public:
    GrammarSymbol head;
    std::vector<GrammarSymbol> body;
    Production();   // no arg constructor generates an isInvalid production with id -1, it is used with hashmap [] operator when nothing is found
    Production(const GrammarSymbol &head, const std::vector<GrammarSymbol> &body);
    Production(int id, const GrammarSymbol &head, const std::vector<GrammarSymbol> &body);
    [[nodiscard]] int getId() const;
    [[nodiscard]] bool isEpsilonProduction() const;
    [[nodiscard]] bool isValid() const;
    bool operator==(const Production &other) const;
    friend std::ostream &operator<<(std::ostream &os, const Production &production);
};

template<>
struct std::hash<Production> {
    std::size_t operator()(const Production &p) const noexcept {
        return std::hash<int>{}(p.getId());
//        size_t hashValue = hash<GrammarSymbol>{}(p.head);
//            for (const auto& symbol : p.body) {
//                hashValue ^= hash<GrammarSymbol>{}(symbol);
//            }
//            return hashValue;
    }
};


#endif //COMPILER_PRODUCTION_H
