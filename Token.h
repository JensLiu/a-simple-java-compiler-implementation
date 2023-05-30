//
// Created by jens on 30/05/23.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H


#include <string>

class Token {
public:
    using TokenType = enum {
        // Keywords
        IF, ELSE, WHILE, RETURN, CLASS, EXTENDS, IMPLEMENTS, NEW, THIS, SUPER, PUBLIC, PRIVATE, PROTECTED, STATIC, PACKAGE,
        // Types
        INT, FLOAT, BOOL, CHAR, STRING, VOID,
        // Literals
        IDENTIFIER, INTEGER_LITERAL, FLOAT_LITERAL, CHAR_LITERAL, STRING_LITERAL, BOOL_LITERAL,
        // Operators
        PLUS, MINUS, STAR, SLASH, PERCENT, CARET, TILDE, AMPERSAND, PIPE, EQUALS, NOT_EQUALS, LESS_THAN, GREATER_THAN,
        LESS_THAN_OR_EQUAL, GREATER_THAN_OR_EQUAL, LOGICAL_AND, LOGICAL_OR, LOGICAL_NOT, ASSIGNMENT, PLUS_ASSIGNMENT,
        MINUS_ASSIGNMENT, STAR_ASSIGNMENT, SLASH_ASSIGNMENT, PERCENT_ASSIGNMENT, CARET_ASSIGNMENT, AMPERSAND_ASSIGNMENT,
        PIPE_ASSIGNMENT, LEFT_SHIFT, RIGHT_SHIFT, LEFT_SHIFT_ASSIGNMENT, RIGHT_SHIFT_ASSIGNMENT, INCREMENT, DECREMENT,
        RIGHT_ARROW,
        // Delimiters
        SEMICOLON, COMMA, DOT, LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET, AT,
        // Special
        END_OF_FILE, ERROR, WHITESPACE
    };

private:
    TokenType tokenType;
    std::string lexeme;

public:
    explicit Token(const TokenType &tokenType);
    Token(const TokenType &tokenType, const std::string &lexeme);
    TokenType getTokenType() const;
    const std::string &getLexeme() const;
};


#endif //COMPILER_TOKEN_H
