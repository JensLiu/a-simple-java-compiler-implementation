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
    Production(const Production &production) = default;
    [[nodiscard]] int getId() const { return this->id; }
    friend std::ostream &operator<<(std::ostream &os, const Production &production);
    bool operator==(const Production &other) const;
    [[nodiscard]] bool isEpsilon() const;
    [[nodiscard]] bool isValid() const;
};

template<>
struct std::hash<Production> {
    std::size_t operator()(const Production &p) const noexcept {
        return std::hash<int>{}(p.getId());
    }
};


#endif //COMPILER_PRODUCTION_H
