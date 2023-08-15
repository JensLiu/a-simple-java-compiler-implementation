//
// Created by jens on 30/05/23.
//

#include "Token.h"
#include <ostream>
#include <iostream>
#include <fstream>

Token::Token(const Token::TokenType &tokenType) {
    this->tokenType = tokenType;
    this->lexeme = "";
    this->line = -1;
    this->column = -1;
}

Token::Token(const Token::TokenType &tokenType, int line, int column) {
    this->tokenType = tokenType;
    this->lexeme = "";
    this->line = line;
    this->column = column;
}

Token::Token(const Token::TokenType &tokenType, const std::string &lexeme, int line, int column) {
    this->tokenType = tokenType;
    this->lexeme = lexeme;
    this->line = line;
    this->column = column;
}

Token::Token(const Token::TokenType &tokenType, const std::string &lexeme, long integerValue, int line, int column) {
    this->tokenType = tokenType;
    this->lexeme = lexeme;
    this->line = line;
    this->column = column;
    this->data.integerValue = integerValue;
}

Token::Token(const Token::TokenType &tokenType, const std::string &lexeme, double longValue, int line, int column) {
    this->tokenType = tokenType;
    this->lexeme = lexeme;
    this->line = line;
    this->column = column;
    this->data.integerValue = longValue;
}

Token::Token(const Token::TokenType &tokenType, const std::string &lexeme, int symbolTableIndex, int line, int column) {
    this->tokenType = tokenType;
    this->lexeme = lexeme;
    this->data.symbolTableIndex = symbolTableIndex;
    this->line = line;
    this->column = column;
}

Token::TokenType Token::getTokenType() const {
    return this->tokenType;
}

const std::string &Token::getLexeme() const {
    return this->lexeme;
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
        {Token::TokenType::IMPORT,                "import"},
        {Token::TokenType::FINAL,                 "final"},
        {Token::TokenType::INT,                   "int"},
        {Token::TokenType::LONG,                  "long"},
        {Token::TokenType::FLOAT,                 "float"},
        {Token::TokenType::DOUBLE,                "double"},
        {Token::TokenType::BOOL,                  "boolean"},
        {Token::TokenType::CHAR,                  "char"},
        {Token::TokenType::VOID,                  "void"},
        {Token::TokenType::FOR,                   "for"},
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
        {Token::TokenType::EQUALS,                "=="},
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
        {Token::TokenType::END_OF_FILE,           "EOF"},
        {Token::TokenType::ERROR,                 "ERROR"},
        {Token::TokenType::WHITESPACE,            "WHITESPACE"},
        {Token::TokenType::NULL_T,                "NULL"},
        {Token::TokenType::EPSILON,               "EPSILON"}
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

int Token::getSymbolTableIndex() const {
    return this->data.symbolTableIndex;
}

Token Token::fromInteger(const std::string &lexeme, int line, int column) {
    char *pEnd = nullptr;
    long val = strtol(lexeme.c_str(), &pEnd, 10);
    if (pEnd != nullptr) {
        return Token(TokenType::INTEGER_LITERAL, lexeme, val, line, column);
    } else {
        throw std::runtime_error("unable to parse lexeme to long");
    }
}

Token Token::fromFloat(const std::string &lexeme, int line, int column) {
    char *pEnd = nullptr;
    double val = strtod(lexeme.c_str(), &pEnd);
    if (pEnd != nullptr) {
        return Token(TokenType::FLOAT_LITERAL, lexeme, val, line, column);
    } else {
        throw std::runtime_error("unable to parse lexeme to long");
    }
}

std::string toBinaryRep(int value) {
    uint32_t *ptr = reinterpret_cast<uint32_t *>(&value);
    uint32_t intValue = *ptr;

    std::string binaryString;
    binaryString.reserve(32);

    for (int i = 31; i >= 0; --i) {
        binaryString.push_back((intValue >> i) & 1 ? '1' : '0');
    }

    return binaryString;
}

std::string toBinaryRep(double value) {
    uint64_t *ptr = reinterpret_cast<uint64_t *>(&value);
    uint64_t intValue = *ptr;

    std::string binaryString;
    binaryString.reserve(64);

    for (int i = 63; i >= 0; --i) {
        binaryString.push_back((intValue >> i) & 1 ? '1' : '0');
    }

    return binaryString;
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
    if (token.getTokenType() == Token::TokenType::IDENTIFIER) {
        return os << "<" << token.getTokenType() << "(" << Token::tokenTypeAsString(token.getTokenType())
                  << "), symbol_table[" << token.getSymbolTableIndex() << "] (lexeme: " << token.lexeme << ")>";
    } else if (token.getTokenType() == Token::TokenType::INTEGER_LITERAL) {
        return os << "<" << token.getTokenType() << "(" << Token::tokenTypeAsString(token.getTokenType())
                  << "), lexeme: " << token.getLexeme() << ", binary: "
                  << toBinaryRep((int) token.data.integerValue) << ">";
    } else if (token.getTokenType() == Token::TokenType::FLOAT_LITERAL) {
        return os << "<" << token.getTokenType() << "(" << Token::tokenTypeAsString(token.getTokenType())
                  << "), lexeme: " << token.getLexeme() << ", binary: " << toBinaryRep((double) token.data.floatValue)
                  << ">";
    }
    return os << "<" << token.getTokenType() << "(" << Token::tokenTypeAsString(token.getTokenType()) << "), "
              << token.lexeme << ">";
}

void Token::exportTokenTypeToCSV(const std::string &filename) {
    std::ofstream fout;
    fout.open(filename, std::ios::out | std::ios::trunc);
    for (int i = 0; i < (int) TokenType::INVALID_TOKEN; i++) {
        fout << i << "," << tokenName.at((TokenType) i) << std::endl;
    }
    fout.close();
}
