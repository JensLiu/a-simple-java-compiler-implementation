//
// Created by jens on 30/05/23.
//

#include <cassert>
#include <iostream>
#include "Lexer.h"
#include "InputBuffer.h"

std::unordered_map<std::string, Token::TokenType> Lexer::KEYWORDS = {
        {"if",         Token::IF},
        {"else",       Token::ELSE},
        {"while",      Token::WHILE},
        {"return",     Token::RETURN},
        {"class",      Token::CLASS},
        {"extends",    Token::EXTENDS},
        {"implements", Token::IMPLEMENTS},
        {"new",        Token::NEW},
        {"this",       Token::THIS},
        {"super",      Token::SUPER},
        {"public",     Token::PUBLIC},
        {"private",    Token::PRIVATE},
        {"protected",  Token::PROTECTED},
        {"static",     Token::STATIC},
        {"int",        Token::INT},
        {"float",      Token::FLOAT},
        {"bool",       Token::BOOL},
        {"char",       Token::CHAR},
        {"string",     Token::STRING},
        {"void",       Token::VOID},
        {"true",       Token::BOOL_LITERAL},
        {"false",      Token::BOOL_LITERAL},
        {"package",    Token::PACKAGE}

};

bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

bool Lexer::mightBeOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!' || c == '&' ||
           c == '|' || c == '^' || c == '~' || c == '%';
}

bool Lexer::isDelimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ';' || c == ',' || c == '.' ||
           c == '@';
}

/**
 * refreshes the token buffer and sets the tokenBufferIndex to 0.
 * @return lexeme
 */
std::string Lexer::commitLexeme() {
    if (forwardIdx >= TOKEN_BUFFER_SIZE)
        throw std::runtime_error("Token buffer overflow");
    tokenBuffer[forwardIdx] = '\0';
    std::string lexeme = std::string(tokenBuffer);
    forwardIdx = 0;
    std::fill(tokenBuffer, tokenBuffer + TOKEN_BUFFER_SIZE, '\0');
    std::cout << "[debug]: lexeme '" << lexeme << "'" << std::endl;
    return lexeme;
}

// NOTE:
// one can get around in this simulated automata by
// (1) using peek() to determine the next state and
// (2) using forward() to consume the character and move to the next state.
// (3) (possibly) using currentChar() to get the current character

/**
 * Returns the next character in the input stream without consuming it.
 * @return next character
 */
char Lexer::peek() {
    char ch = inputBuffer->peek();
    return ch;
}

/**
 * Consumes the next character in the input stream.
 * also adds it to the token buffer.
 */
void Lexer::forward() {
    inputBuffer->next();
    char ch = inputBuffer->getChar();
    if (forwardIdx >= TOKEN_BUFFER_SIZE)
        throw std::runtime_error("Token buffer overflow");
    tokenBuffer[forwardIdx++] = ch;
}


void Lexer::forwardIgnore() {
    inputBuffer->next();
}


/**
 * Returns the current character in the input stream.
 * @return
 */
char Lexer::currentChar() {
    return tokenBuffer[forwardIdx - 1];
}


// NOTE:
// main routines are named as handle<state> and return a token
// when calling main routines, the current character is not consumed (we are just choosing which state to go to)
// which means that the automata is before the start node: YOU_ARE_HERE -> StartState --(ch)--> NextState
// when calling subroutines, the current character is consumed (we are moving to the next state)
Token Lexer::nextToken() {

    char ch = peek();
    if (isLetter(ch) || ch == '_' || ch == '$') {
        return handleIdentifier();
    } else if (isDigit(ch)) {
        return handleNumber();
    } else if (ch == '"') {
        return handleStringLiteral();
    } else if (mightBeOperator(ch)) {
        return handleOperator();
    } else if (isDelimiter(ch)) {
        return handleDelimiter();
    } else if (isWhitespace(ch)) {
        return handleWhitespace();
    } else if (ch == EOF) {
        return Token(Token::TokenType::END_OF_FILE);
    } else {
        throw std::runtime_error("Unexpected character");
    }

}

Token Lexer::handleDelimiter() {
    // <delimiter> ::= ( | ) | { | } | [ | ] | ; | , | . | @
    assert(isDelimiter(peek()));
    forward();
    switch (currentChar()) {
        case '(':
            commitLexeme();
            return Token(Token::TokenType::LEFT_PAREN);
        case ')':
            commitLexeme();
            return Token(Token::TokenType::RIGHT_PAREN);
        case '{':
            commitLexeme();
            return Token(Token::TokenType::LEFT_BRACE);
        case '}':
            commitLexeme();
            return Token(Token::TokenType::RIGHT_BRACE);
        case '[':
            commitLexeme();
            return Token(Token::TokenType::LEFT_BRACKET);
        case ']':
            commitLexeme();
            return Token(Token::TokenType::RIGHT_BRACKET);
        case ';':
            commitLexeme();
            return Token(Token::TokenType::SEMICOLON);
        case ',':
            commitLexeme();
            return Token(Token::TokenType::COMMA);
        case '.':
            commitLexeme();
            return Token(Token::TokenType::DOT);
        case '@':
            commitLexeme();
            return Token(Token::TokenType::AT);
        default:
            throw std::runtime_error("Unexpected delimiter");
    }
}

Token Lexer::handleIdentifier() {
    // <identifier> ::= [a-zA-Z_][a-zA-Z0-9_]*
    assert(isLetter(peek()) || peek() == '_' || peek() == '$');
    forward();
    while (isDigit(peek()) || isLetter(peek()) || peek() == '_' || peek() == '$') {
        forward();
    }
    std::string lexeme = commitLexeme();
    if (KEYWORDS.find(lexeme) != KEYWORDS.end()) {
        return Token(KEYWORDS[lexeme]);
    }
    return Token(Token::TokenType::IDENTIFIER, lexeme);
}

Token Lexer::handleNumber() {
    // <opt-exp> ::= [Ee][+-]?[0-9]+ | epsilon
    // <opt-frac> ::= .([0-9])+ | epsilon
    // <number> ::= <digits> <opt-frac> <opt-exp>

    // NOTE
    // we do not allow + or - to appear in our description of number
    // because it cannot distinguish between a signed number and an arithmetic expression

    assert(isDigit(peek()));
    forward();

    while (isDigit(peek())) {
        forward();
    }

    if (peek() == '.' || peek() == 'e' || peek() == 'E') {
        // floating point number
        if (peek() == '.') {
            forward();
            handleOptionalFractionSubroutine();
        }
        if (peek() == 'e' || peek() == 'E') {
            forward();
            handleOptionalExponentSubroutine();
        }
        return Token(Token::TokenType::FLOAT_LITERAL, commitLexeme());
    } else {
        // integer number
        return Token(Token::TokenType::INTEGER_LITERAL, commitLexeme());
    }


}

void Lexer::handleOptionalFractionSubroutine() {
    // <opt-frac> ::= .([0-9])+ | epsilon
    assert(currentChar() == '.');

    if (!isDigit(peek())) {
        // got .<non-digit> in a number
        throw std::runtime_error("Invalid number");
    }

    // we got at least one digit
    while (isDigit(peek())) {
        forward();
    }
}

void Lexer::handleOptionalExponentSubroutine() {
    // <opt-exp> ::= [Ee][+-]?[0-9]+ | epsilon
    assert(currentChar() == 'e' || currentChar() == 'E');

    if (peek() == '+' || peek() == '-') {
        forward();
    }

    if (!isDigit(peek())) {
        // got [Ee][+-]?<non-digit> in a number
        throw std::runtime_error("Invalid number");
    }

    // we got at least one digit
    while (isDigit(peek())) {
        forward();
    }
}


Token Lexer::handleWhitespace() {
    assert(isWhitespace(peek()));
    forwardIgnore();
    while (isWhitespace(peek())) {
        forwardIgnore();
    }
    commitLexeme();
    return Token(Token::TokenType::WHITESPACE);
}


Token Lexer::handleStringLiteral() {
    // <string-literal-char> ::= ^['\]
    // <string-literal> ::= " <string-literal-char>* "
    assert(peek() == '"');
    forward();
    while (peek() != '"') {
        if (peek() == '\\') {
            forward();
            handleEscapeSubroutine();
        } else {
            forward();
        }
    }
    forward();
    return Token(Token::TokenType::STRING_LITERAL, commitLexeme());
}

Token Lexer::handleCharLiteral() {
    // <char-literal-char> ::= ^['\]
    // <char-literal> ::= ' <char-literal-char> '
    assert(peek() == '\'');
    forward();
    if (peek() == '\\') {
        forward();
        handleEscapeSubroutine();
        return Token(Token::TokenType::CHAR_LITERAL, commitLexeme());
    } else {
        forward();
        return Token(Token::TokenType::CHAR_LITERAL, commitLexeme());
    }
}


/**
 * this function overrides the default behaviour of forward() by replacing
 * the \ character in tokenBuffer with the actual escaped character.
 * (otherwise we have two characters in the tokenBuffer, \ and the escaped character)
 */
void Lexer::handleEscapeSubroutine() {
    assert(currentChar() == '\\');
    char ch = peek();
    if (ch == 'n' || ch == 't' || ch == 'r' || ch == 'b' || ch == 'f' || ch == '"' || ch == '\'' || ch == '\\') {
        char original;
        switch (ch) {
            case 'n':
                original = '\n';
                break;
            case 't':
                original = '\t';
                break;
            case 'r':
                original = '\r';
                break;
            case 'b':
                original = '\b';
                break;
            case 'f':
                original = '\f';
                break;
            case '"':
                original = '"';
                break;
            case '\'':
                original = '\'';
                break;
            case '\\':
                original = '\\';
                break;
            default:
                throw std::runtime_error("Invalid escape sequence");
        }
        tokenBuffer[forwardIdx - 1] = original;
        inputBuffer->next();
    } else {
        throw std::runtime_error("Invalid escape sequence");
    }
}

void Lexer::handleSingleLineCommentSubroutine() {
    assert(currentChar() == '/');
    std::cout << "handling single line comment" << std::endl;
    while (peek() != '\n') {
        forwardIgnore();
    }
}

void Lexer::handleMultiLineCommentSubroutine() {
    assert(currentChar() == '*');
    while (true) {
        forwardIgnore();
        if (currentChar() == '*' && peek() == '/') {
            forwardIgnore();
            break;
        }
    }
}

Token Lexer::handleOperator() {
    assert(mightBeOperator(peek()));
    forward();

    char ch = currentChar();

    if (ch == '+') {
        if (peek() == '+') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::INCREMENT);
        } else if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::PLUS_ASSIGNMENT);
        } else {
            commitLexeme();
            return Token(Token::TokenType::PLUS);
        }
    } else if (ch == '-') {
        if (peek() == '-') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::DECREMENT);
        } else if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::MINUS_ASSIGNMENT);
        } else if (peek() == '>') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::RIGHT_ARROW);
        } else {
            commitLexeme();
            return Token(Token::TokenType::MINUS);
        }
    } else if (ch == '*') {
        if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::STAR_ASSIGNMENT);
        } else {
            commitLexeme();
            return Token(Token::TokenType::STAR);
        }
    } else if (ch == '/') {
        if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::SLASH_ASSIGNMENT);
        } else if (peek() == '/') {     // single line comment
            forward();
            handleSingleLineCommentSubroutine();
            commitLexeme();
            return Token(Token::TokenType::WHITESPACE);
        } else if (peek() == '*') {     // multi line comment
            forward();
            handleMultiLineCommentSubroutine();
            commitLexeme();
            return Token(Token::TokenType::WHITESPACE);
        } else {
            commitLexeme();
            return Token(Token::TokenType::SLASH);
        }
    } else if (ch == '=') {
        if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::EQUALS);
        } else {
            commitLexeme();
            return Token(Token::TokenType::ASSIGNMENT);
        }
    } else if (ch == '<') {
        if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::LESS_THAN_OR_EQUAL);
        } else {
            commitLexeme();
            return Token(Token::TokenType::LESS_THAN);
        }
    } else if (ch == '>') {
        if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::GREATER_THAN_OR_EQUAL);
        } else {
            commitLexeme();
            return Token(Token::TokenType::GREATER_THAN);
        }
    } else if (ch == '!') {
        if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::NOT_EQUALS);
        } else {
            commitLexeme();
            return Token(Token::TokenType::LOGICAL_NOT);
        }
    } else if (ch == '&') {
        if (peek() == '&') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::LOGICAL_AND);
        } else {
            commitLexeme();
            return Token(Token::TokenType::AMPERSAND);
        }
    } else if (ch == '|') {
        if (peek() == '|') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::LOGICAL_OR);
        } else if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::PIPE_ASSIGNMENT);
        } else {
            commitLexeme();
            return Token(Token::TokenType::PIPE);
        }
    } else if (ch == '^') {
        if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::CARET_ASSIGNMENT);
        } else {
            commitLexeme();
            return Token(Token::TokenType::CARET);
        }
    } else if (ch == '~') {
        commitLexeme();
        return Token(Token::TokenType::TILDE);
    } else if (ch == '%') {
        if (peek() == '=') {
            commitLexeme();
            return Token(Token::TokenType::PERCENT_ASSIGNMENT);
        } else {
            commitLexeme();
            return Token(Token::TokenType::PERCENT);
        }
    } else {
        throw std::runtime_error("Invalid operator");
    }
}

Lexer::Lexer(InputBuffer *inputBuffer) {
    this->inputBuffer = inputBuffer;
//    this->tokenBuffer = new char[TOKEN_BUFFER_SIZE];
}