//
// Created by jens on 30/05/23.
//

#include "Token.h"

#include <utility>

Token::Token(const Token::TokenType& tokenType, const std::string& lexeme) {
    this->tokenType = tokenType;
    this->lexeme = std::move(lexeme);
}

Token::Token(const Token::TokenType& tokenType) {
    this->tokenType = tokenType;
    this->lexeme = "";
}

Token::TokenType Token::getTokenType() const {
    return this->tokenType;
}

const std::string &Token::getLexeme() const {
    return this->lexeme;
}
