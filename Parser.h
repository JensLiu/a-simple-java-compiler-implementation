//
// Created by jens on 02/06/23.
//

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H


#include "ContextFreeGrammar.h"
#include "SymbolTable.h"
#include "Lexer.h"
#include <stdexcept>

class Parser {
private:
    ContextFreeGrammar grammar;
    SymbolTable *symbolTable;
    Lexer *lexer;
public:
    Parser(const ContextFreeGrammar &grammar, Lexer *lexer, SymbolTable *symboltable);
    void parse();
};

class SyntacticalError : public std::runtime_error {
private:
    int line;
    int column;
    std::string message;
public:
    SyntacticalError(const std::string &error, int line, int column)
            : std::runtime_error(error), line(line + 1), column(column + 1) {
        this->message = error + " at line " + std::to_string(line + 1) + " at column " + std::to_string(column + 1);
    }

    [[nodiscard]] const char *what() const noexcept override {
        return message.c_str();
    }

    [[nodiscard]] int getLine() const { return this->line; }

    [[nodiscard]] int getColumn() const { return this->column; }
};


#endif //COMPILER_PARSER_H
