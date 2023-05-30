//
// Created by jens on 30/05/23.
//

#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include <unordered_map>
#include "Token.h"
#include "InputBuffer.h"

class Lexer {
public:
    static bool isDigit(char c);
    static bool isLetter(char c);
    static bool isWhitespace(char c);
    static bool mightBeOperator(char c);
    static const int TOKEN_BUFFER_SIZE = 1024;

public:
    explicit Lexer(InputBuffer *inputBuffer);

private:
    InputBuffer *inputBuffer;
    char tokenBuffer[TOKEN_BUFFER_SIZE]{};
    int forwardIdx = 0;
    char currentChar();
    char peek();
    void forward();
    void forwardIgnore();
    std::string commitLexeme();

    Token handleIdentifier();
    Token handleNumber();
    void handleOptionalFractionSubroutine();
    void handleOptionalExponentSubroutine();
    Token handleOperator();
    Token handleWhitespace();

    Token handleStringLiteral();
    void handleEscapeSubroutine();

    void handleSingleLineCommentSubroutine();

    void handleMultiLineCommentSubroutine();

public:
    Token nextToken();
    Token handleCharLiteral();
    static std::unordered_map<std::string, Token::TokenType> KEYWORDS;

    Token handleDelimiter();

    static bool isDelimiter(char c);
};


#endif //COMPILER_LEXER_H
