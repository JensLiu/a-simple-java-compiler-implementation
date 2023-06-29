//
// Created by jens on 02/06/23.
//

#ifndef COMPILER_GRAMMAR_DEF_H
#define COMPILER_GRAMMAR_DEF_H

#include <vector>
#include "Production.h"


std::vector<Production> grammarDefs =
        {
                // ---------------------------------------expression grammar -------------------------------------------
                // <expr> ::= <term> <expr_p>
                Production(HEAD("<expr>"), {NT("<term>"), NT("<expr_p>")}),
                // <expr_p> ::= + <term> <expr_p> | - <term> <expr_p> | epsilon
                Production(HEAD("<expr_p>"), {T(Token::PLUS), NT("<term>"), NT("<expr_p>")}),
                Production(HEAD("<expr_p>"), {T(Token::MINUS), NT("<term>"), NT("<expr_p>")}),
                Production(HEAD("<expr_p>"), {GrammarSymbol::epsilon()}),
                // <term> ::= <factor> <term_p>
                Production(HEAD("<term>"), {NT("<factor>"), NT("<term_p>")}),
                // <term_p> ::= * <factor> <term_p> | / <factor> <term_p> | % <factor> <term_p> | epsilon
                Production(HEAD("<term_p>"), {T(Token::STAR), NT("<factor>"), NT("<term_p>")}),
                Production(HEAD("<term_p>"), {T(Token::SLASH), NT("<factor>"), NT("<term_p>")}),
                Production(HEAD("<term_p>"), {T(Token::PERCENT), NT("<factor>"), NT("<term_p>")}),
                Production(HEAD("<term_p>"), {GrammarSymbol::epsilon()}),
                // <factor> ::= ( <expr> ) | <int_lit> | <float_lit> | <id>
                Production(HEAD("<factor>"), {T(Token::LEFT_PAREN), NT("<expr>"), T(Token::RIGHT_PAREN)}),
                Production(HEAD("<factor>"), {T(Token::INTEGER_LITERAL)}),
                Production(HEAD("<factor>"), {T(Token::FLOAT_LITERAL)}),
                Production(HEAD("<factor>"), {T(Token::IDENTIFIER)}),
                // -------------------------------------- statement grammar --------------------------------------------
                Production(HEAD("<>"), {})
        };

#endif //COMPILER_GRAMMAR_DEF_H
