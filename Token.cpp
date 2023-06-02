//
// Created by jens on 30/05/23.
//

#include "Token.h"
#include <ostream>

Token::Token(const Token::TokenType &tokenType) {
    this->tokenType = tokenType;
    this->lexeme = "";
    this->line = -1;
    this->column = -1;
//    this->symbolTableIndex = -1;
}

Token::Token(const Token::TokenType &tokenType, int line, int column) {
    this->tokenType = tokenType;
    this->lexeme = "";
    this->line = line;
    this->column = column;
//    this->symbolTableIndex = -1;
}

Token::Token(const Token::TokenType &tokenType, const std::string &lexeme, int line, int column) {
    this->tokenType = tokenType;
    this->lexeme = lexeme;
    this->line = line;
    this->column = column;
}

//Token::Token(const Token::TokenType &tokenType, const std::string &lexeme, int symbolTableIndex, int line, int column) {
//    this->tokenType = tokenType;
//    this->lexeme = lexeme;
//    this->symbolTableIndex = symbolTableIndex;
//    this->line = line;
//    this->column = column;
//}

Token::TokenType Token::getTokenType() const {
    return this->tokenType;
}

const std::string &Token::getLexeme() const {
    return this->lexeme;
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
    return os << "<" << token.getTokenType() << "(" << Token::tokenTypeAsString(token.getTokenType()) << "), "
              << token.lexeme << ">";
}

const std::unordered_map<Token::TokenType, std::string> Token::tokenName = {
        {Token::TokenType::INVALID_TOKEN,         "INVALID_TOKEN"},
        {Token::TokenType::IF,                    "if"},
        {Token::TokenType::ELSE,                  "else"},
        {Token::TokenType::WHILE,                 "while"},
        {Token::TokenType::RETURN,                "return"},
        {Token::TokenType::CLASS,                 "class"},
        {Token::TokenType::EXTENDS,               "extends"},
        {Token::TokenType::IMPLEMENTS,            "implements"},
        {Token::TokenType::NEW,                   "new"},
        {Token::TokenType::THIS,                  "this"},
        {Token::TokenType::SUPER,                 "super"},
        {Token::TokenType::PUBLIC,                "public"},
        {Token::TokenType::PRIVATE,               "private"},
        {Token::TokenType::PROTECTED,             "protected"},
        {Token::TokenType::STATIC,                "static"},
        {Token::TokenType::PACKAGE,               "package"},
        {Token::TokenType::INT,                   "int"},
        {Token::TokenType::FLOAT,                 "float"},
        {Token::TokenType::BOOL,                  "bool"},
        {Token::TokenType::CHAR,                  "char"},
        {Token::TokenType::STRING,                "string"},
        {Token::TokenType::VOID,                  "void"},
        {Token::TokenType::IDENTIFIER,            "id"},
        {Token::TokenType::INTEGER_LITERAL,       "int_literal"},
        {Token::TokenType::FLOAT_LITERAL,         "float_literal"},
        {Token::TokenType::CHAR_LITERAL,          "char_literal"},
        {Token::TokenType::STRING_LITERAL,        "string_literal"},
        {Token::TokenType::BOOL_LITERAL,          "bool_literal"},
        {Token::TokenType::PLUS,                  "+"},
        {Token::TokenType::MINUS,                 "-"},
        {Token::TokenType::STAR,                  "*"},
        {Token::TokenType::SLASH,                 "/"},
        {Token::TokenType::PERCENT,               "%"},
        {Token::TokenType::CARET,                 "^"},
        {Token::TokenType::TILDE,                 "~"},
        {Token::TokenType::AMPERSAND,             "&"},
        {Token::TokenType::PIPE,                  "|"},
        {Token::TokenType::EQUALS,                "="},
        {Token::TokenType::NOT_EQUALS,            "!="},
        {Token::TokenType::LESS_THAN,             "<"},
        {Token::TokenType::GREATER_THAN,          ">"},
        {Token::TokenType::LESS_THAN_OR_EQUAL,    "<="},
        {Token::TokenType::GREATER_THAN_OR_EQUAL, ">="},
        {Token::TokenType::LOGICAL_AND,           "&"},
        {Token::TokenType::LOGICAL_OR,            "|"},
        {Token::TokenType::LOGICAL_NOT,           "!"},
        {Token::TokenType::INCREMENT,             "++"},
        {Token::TokenType::DECREMENT,             "--"},
        {Token::TokenType::ASSIGNMENT,            "="},
        {Token::TokenType::PLUS_ASSIGNMENT,       "+="},
        {Token::TokenType::MINUS_ASSIGNMENT,      "-="},
        {Token::TokenType::STAR_ASSIGNMENT,       "*="},
        {Token::TokenType::SLASH_ASSIGNMENT,      "/="},
        {Token::TokenType::PERCENT_ASSIGNMENT,    "%="},
        {Token::TokenType::CARET_ASSIGNMENT,      "^="},
//        {Token::TokenType::TILDE_ASSIGNMENT, "TILDE_ASSIGNMENT"},
        {Token::TokenType::AMPERSAND_ASSIGNMENT,  "&="},
        {Token::TokenType::PIPE_ASSIGNMENT,       "|="},
        {Token::TokenType::LEFT_PAREN,            "("},
        {Token::TokenType::RIGHT_PAREN,           ")"},
        {Token::TokenType::LEFT_BRACE,            "{"},
        {Token::TokenType::RIGHT_BRACE,           "}"},
        {Token::TokenType::LEFT_BRACKET,          "["},
        {Token::TokenType::RIGHT_BRACKET,         "]"},
        {Token::TokenType::SEMICOLON,             ";"},
        {Token::TokenType::COMMA,                 ","},
        {Token::TokenType::DOT,                   "."},
//        {Token::TokenType::COLON, "COLON"},
//        {Token::TokenType::QUESTION_MARK, "QUESTION_MARK"},
        {Token::TokenType::END_OF_FILE,           "EOF"},
        {Token::TokenType::ERROR,                 "ERROR"},
        {Token::TokenType::WHITESPACE,            "WHITESPACE"},
        {Token::TokenType::NULL_T,                "null"},
        {Token::TokenType::EPSILON,               "ϵ"}
};

std::string Token::tokenTypeAsString(const Token::TokenType &tokenType) {
    return tokenName.at(tokenType);
}

int Token::getLine() const {
    return this->line;
}

int Token::getColumn() const {
    return this->column;
}

bool Token::isEOF() const {
    return this->tokenType == TokenType::END_OF_FILE;
}

bool Token::isWhitespace() const {
    return this->tokenType == TokenType::WHITESPACE;
}
