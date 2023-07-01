//
// Created by jens on 30/05/23.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

#include <unordered_map>
#include <string>

class Token {
public:
    using TokenType = enum {
        // Keywords
        IF, ELSE, WHILE, RETURN, CLASS, EXTENDS, IMPLEMENTS, NEW, THIS, SUPER, PUBLIC, PRIVATE, PROTECTED, STATIC, PACKAGE, FINAL,
        IMPORT,
        // Types
        INT, FLOAT, BOOL, CHAR, STRING, VOID, NULL_T,
        // Literals
        IDENTIFIER, INTEGER_LITERAL, FLOAT_LITERAL, CHAR_LITERAL, STRING_LITERAL, BOOL_LITERAL,
        // Operators
        PLUS, MINUS, STAR, SLASH, PERCENT, CARET, TILDE, AMPERSAND, PIPE, EQUALS, NOT_EQUAL, LESS_THAN, GREATER_THAN,
        LESS_THAN_OR_EQUAL, GREATER_THAN_OR_EQUAL, LOGICAL_AND, LOGICAL_OR, LOGICAL_NOT, ASSIGNMENT, PLUS_ASSIGNMENT,
        MINUS_ASSIGNMENT, STAR_ASSIGNMENT, SLASH_ASSIGNMENT, PERCENT_ASSIGNMENT, CARET_ASSIGNMENT, AMPERSAND_ASSIGNMENT,
        PIPE_ASSIGNMENT, LEFT_SHIFT, RIGHT_SHIFT, LEFT_SHIFT_ASSIGNMENT, RIGHT_SHIFT_ASSIGNMENT, INCREMENT, DECREMENT,
        RIGHT_ARROW,
        // Delimiters
        SEMICOLON, COMMA, DOT, LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET, AT,
        // Special
        END_OF_FILE, ERROR, WHITESPACE,
        // to be used in the parser
        EPSILON, INVALID_TOKEN
    };

    static const std::unordered_map<TokenType, std::string> tokenName;

    static std::string tokenTypeAsString(const TokenType &tokenType);

private:
    std::string lexeme;
    TokenType tokenType;
    union {
        long integerValue;
        double floatValue;
        int symbolTableIndex;
    } data;

    int line;
    int column;

    // for numbers
    Token(const TokenType &tokenType, const std::string &lexeme, long integerValue, int line, int column);
    Token(const TokenType &tokenType, const std::string &lexeme, double floatValue, int line, int column);

public:
    explicit Token(const TokenType &tokenType);     // this is used when token is treated as a terminal in grammar
    Token(const TokenType &tokenType, const std::string &lexeme, int line, int column);    // for string literals
    Token(const TokenType &tokenType, const std::string &lexeme, int symbolTableIndex, int line,
          int column);   // for identifiers
    Token(const TokenType &tokenType, int line, int column);    // others

    static Token fromInteger(const std::string &lexeme, int line, int column);
    static Token fromFloat(const std::string &lexeme, int line, int column);

    [[nodiscard]] TokenType getTokenType() const;

    [[nodiscard]] const std::string &getLexeme() const;

    [[nodiscard]] int getLine() const;

    [[nodiscard]] int getColumn() const;

    [[nodiscard]] int getSymbolTableIndex() const;

    [[nodiscard]] bool isEOF() const;

    [[nodiscard]] bool isWhitespace() const;

    friend std::ostream &operator<<(std::ostream &os, const Token &token);

    static void exportTokenTypeToCSV(const std::string &filename);

};


#endif //COMPILER_TOKEN_H
