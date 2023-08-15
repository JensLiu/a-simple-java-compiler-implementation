//
// Created by jens on 02/06/23.
//

#include <stack>
#include <iostream>
#include "Parser.h"

#define PRINT_STACK(stack) std::cout << "\tstack: "; for (int i = 0; i < stack.size(); i++) std::cout << stack[i] << " "; std::cout << std::endl;

void Parser::parse() {
    std::vector<GrammarSymbol> stack;
    stack.push_back(GrammarSymbol::eof());  // add end marker to represent the bottom of the stack
    stack.push_back(grammar.getStartSymbol());
    while (!stack.empty()) {
        const Token &token = lexer->nextToken();
        std::cout << "got token: " << token << std::endl;

        if (token.isWhitespace()) continue; // ignoring whitespaces such as space, tab, newline
        while (true) {
            const GrammarSymbol &node = stack.back();
            PRINT_STACK(stack)
            // create symbol using the current token in order to use prediction
            GrammarSymbol inputSymbol = GrammarSymbol::createTerminal(token.getTokenType());

            if (node.isTerminal() && node == inputSymbol) {
                stack.pop_back();
                break;
            } else if (node.isTerminal() && !node.isEpsilon()) {
                throw SyntacticalError("error: expected " + node.toString() + "got" + inputSymbol.toString() , token.getLine(), token.getColumn());
            }

            // use parsing table to predict the next production
            std::cout << "\t\tpredict: " << node << " on " << inputSymbol << std::endl;
            std::variant<Production, ErrorStrategy> result = grammar.predict(node, inputSymbol);

            if (std::holds_alternative<Production>(result)) {
                // use the predicted production to preceded
                const Production &p = std::get<Production>(result);
                stack.pop_back();
                std::cout << "\t\texpand using: " << p << std::endl;
                for (int i = p.body.size() - 1; i >= 0; i--) {
                    // push to stack in reverse since left-most derivation
                    if (!p.body[i].isEpsilon()) {
                        stack.push_back(p.body[i]);
                    }
                }
            } else if (std::holds_alternative<ErrorStrategy>(result)) {
                // on error, might do recovery, isn't implemented
                throw SyntacticalError("error", token.getLine(), token.getColumn());
            }

        }
    }
    std::cout << "accept" << std::endl;
}

Parser::Parser(const ContextFreeGrammar &grammar, Lexer *lexer, SymbolTable *symbolTable)
        : grammar(grammar), lexer(lexer), symbolTable(symbolTable) {}