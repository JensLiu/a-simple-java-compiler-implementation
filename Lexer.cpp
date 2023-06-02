//
// Created by jens on 30/05/23.
//

#include <cassert>
#include <iostream>
#include "Lexer.h"
#include "InputBuffer.h"

#define THROW_LEXICAL_ERROR(message) throw LexicalError(message, inputBuffer->getLine(), inputBuffer->getColumn())

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
        {"package",    Token::PACKAGE},
        {"null",       Token::NULL_T}
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

bool Lexer::isOperator(char c) {
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
    } else if (isOperator(ch)) {
        return handleOperator();
    } else if (isDelimiter(ch)) {
        return handleDelimiter();
    } else if (isWhitespace(ch)) {
        return handleWhitespace();
    } else if (ch == '\'') {
        return handleCharLiteral();
    } else if (ch == EOF) {
        return Token(Token::TokenType::END_OF_FILE);
    } else {
        THROW_LEXICAL_ERROR("unexpected character");
    }

}

Token Lexer::handleDelimiter() {
    // <delimiter> ::= ( | ) | { | } | [ | ] | ; | , | . | @
    assert(isDelimiter(peek()));
    forward();
    switch (currentChar()) {
        case '(':
            commitLexeme();
            return Token(Token::TokenType::LEFT_PAREN, inputBuffer->getLine(), inputBuffer->getColumn());
        case ')':
            commitLexeme();
            return Token(Token::TokenType::RIGHT_PAREN, inputBuffer->getLine(), inputBuffer->getColumn());
        case '{':
            commitLexeme();
            return Token(Token::TokenType::LEFT_BRACE, inputBuffer->getLine(), inputBuffer->getColumn());
        case '}':
            commitLexeme();
            return Token(Token::TokenType::RIGHT_BRACE, inputBuffer->getLine(), inputBuffer->getColumn());
        case '[':
            commitLexeme();
            return Token(Token::TokenType::LEFT_BRACKET, inputBuffer->getLine(), inputBuffer->getColumn());
        case ']':
            commitLexeme();
            return Token(Token::TokenType::RIGHT_BRACKET, inputBuffer->getLine(), inputBuffer->getColumn());
        case ';':
            commitLexeme();
            return Token(Token::TokenType::SEMICOLON, inputBuffer->getLine(), inputBuffer->getColumn());
        case ',':
            commitLexeme();
            return Token(Token::TokenType::COMMA, inputBuffer->getLine(), inputBuffer->getColumn());
        case '.':
            commitLexeme();
            return Token(Token::TokenType::DOT, inputBuffer->getLine(), inputBuffer->getColumn());
        case '@':
            commitLexeme();
            return Token(Token::TokenType::AT, inputBuffer->getLine(), inputBuffer->getColumn());
        default:
            THROW_LEXICAL_ERROR("Unexpected delimiter");
    }
}

Token Lexer::handleIdentifier() {
    // <identifier> ::= [a-zA-Z_$][a-zA-Z0-9_$]*
    assert(isLetter(peek()) || peek() == '_' || peek() == '$');
    forward();
    while (isDigit(peek()) || isLetter(peek()) || peek() == '_' || peek() == '$') {
        forward();
    }
    std::string lexeme = commitLexeme();
    if (KEYWORDS.find(lexeme) != KEYWORDS.end()) {
        return Token(KEYWORDS[lexeme]);
    }
    return Token(Token::TokenType::IDENTIFIER, lexeme, inputBuffer->getLine(), inputBuffer->getColumn());
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
        return Token(Token::TokenType::FLOAT_LITERAL, commitLexeme(), inputBuffer->getLine(), inputBuffer->getColumn());
    } else {
        // integer number
        return Token(Token::TokenType::INTEGER_LITERAL, commitLexeme(), inputBuffer->getLine(), inputBuffer->getColumn());
    }


}

void Lexer::handleOptionalFractionSubroutine() {
    // <opt-frac> ::= .([0-9])+ | epsilon
    assert(currentChar() == '.');

    if (!isDigit(peek())) {
        // got .<non-digit> in a number
        THROW_LEXICAL_ERROR("Invalid number");
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
        THROW_LEXICAL_ERROR("Invalid number");
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
    return Token(Token::TokenType::WHITESPACE, inputBuffer->getLine(), inputBuffer->getColumn());
}


Token Lexer::handleStringLiteral() {
    // <string-literal-char> ::= ^['\]
    // <string-literal> ::= " <string-literal-char>* "
    assert(peek() == '"');
    forwardIgnore();    // do not take the first "
    while (peek() != '"') {
        if (peek() == '\\') {
            forward();
            handleEscapeSubroutine();
        } else {
            forward();
        }
    }
    forwardIgnore();
    return Token(Token::TokenType::STRING_LITERAL, commitLexeme(), inputBuffer->getLine(), inputBuffer->getColumn());
}

Token Lexer::handleCharLiteral() {
    // <char-literal-char> ::= ^['\]
    // <char-literal> ::= ' <char-literal-char> '
    assert(peek() == '\'');
    forwardIgnore();
    if (peek() == '\\') {
        forward();
        handleEscapeSubroutine();
        return Token(Token::TokenType::CHAR_LITERAL, commitLexeme(), inputBuffer->getLine(), inputBuffer->getColumn());
    } else {
        forward();
        return Token(Token::TokenType::CHAR_LITERAL, commitLexeme(), inputBuffer->getLine(), inputBuffer->getColumn());
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
                THROW_LEXICAL_ERROR("Invalid escape sequence");
        }
        tokenBuffer[forwardIdx - 1] = original;
        inputBuffer->next();
    } else {
        THROW_LEXICAL_ERROR("Invalid escape sequence");
    }
}

void Lexer::handleSingleLineCommentSubroutine() {
    assert(currentChar() == '/');
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
    assert(isOperator(peek()));
    forward();

    char ch = currentChar();

    if (ch == '+') {
        if (peek() == '+') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::INCREMENT, inputBuffer->getLine(), inputBuffer->getColumn());
        } else if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::PLUS_ASSIGNMENT, inputBuffer->getLine(), inputBuffer->getColumn());
        } else {
            commitLexeme();
            return Token(Token::TokenType::PLUS, inputBuffer->getLine(), inputBuffer->getColumn());
        }
    } else if (ch == '-') {
        if (peek() == '-') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::DECREMENT, inputBuffer->getLine(), inputBuffer->getColumn());
        } else if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::MINUS_ASSIGNMENT, inputBuffer->getLine(), inputBuffer->getColumn());
        } else if (peek() == '>') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::RIGHT_ARROW, inputBuffer->getLine(), inputBuffer->getColumn());
        } else {
            commitLexeme();
            return Token(Token::TokenType::MINUS, inputBuffer->getLine(), inputBuffer->getColumn());
        }
    } else if (ch == '*') {
        if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::STAR_ASSIGNMENT, inputBuffer->getLine(), inputBuffer->getColumn());
        } else {
            commitLexeme();
            return Token(Token::TokenType::STAR, inputBuffer->getLine(), inputBuffer->getColumn());
        }
    } else if (ch == '/') {
        if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::SLASH_ASSIGNMENT, inputBuffer->getLine(), inputBuffer->getColumn());
        } else if (peek() == '/') {     // single line comment
            forward();
            handleSingleLineCommentSubroutine();
            commitLexeme();
            return Token(Token::TokenType::WHITESPACE, inputBuffer->getLine(), inputBuffer->getColumn());
        } else if (peek() == '*') {     // multi line comment
            forward();
            handleMultiLineCommentSubroutine();
            commitLexeme();
            return Token(Token::TokenType::WHITESPACE, inputBuffer->getLine(), inputBuffer->getColumn());
        } else {
            commitLexeme();
            return Token(Token::TokenType::SLASH, inputBuffer->getLine(), inputBuffer->getColumn());
        }
    } else if (ch == '=') {
        if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::EQUALS, inputBuffer->getLine(), inputBuffer->getColumn());
        } else {
            commitLexeme();
            return Token(Token::TokenType::ASSIGNMENT, inputBuffer->getLine(), inputBuffer->getColumn());
        }
    } else if (ch == '<') {
        if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::LESS_THAN_OR_EQUAL, inputBuffer->getLine(), inputBuffer->getColumn());
        } else {
            commitLexeme();
            return Token(Token::TokenType::LESS_THAN, inputBuffer->getLine(), inputBuffer->getColumn());
        }
    } else if (ch == '>') {
        if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::GREATER_THAN_OR_EQUAL, inputBuffer->getLine(), inputBuffer->getColumn());
        } else {
            commitLexeme();
            return Token(Token::TokenType::GREATER_THAN, inputBuffer->getLine(), inputBuffer->getColumn());
        }
    } else if (ch == '!') {
        if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::NOT_EQUALS, inputBuffer->getLine(), inputBuffer->getColumn());
        } else {
            commitLexeme();
            return Token(Token::TokenType::LOGICAL_NOT, inputBuffer->getLine(), inputBuffer->getColumn());
        }
    } else if (ch == '&') {
        if (peek() == '&') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::LOGICAL_AND, inputBuffer->getLine(), inputBuffer->getColumn());
        } else {
            commitLexeme();
            return Token(Token::TokenType::AMPERSAND, inputBuffer->getLine(), inputBuffer->getColumn());
        }
    } else if (ch == '|') {
        if (peek() == '|') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::LOGICAL_OR, inputBuffer->getLine(), inputBuffer->getColumn());
        } else if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::PIPE_ASSIGNMENT, inputBuffer->getLine(), inputBuffer->getColumn());
        } else {
            commitLexeme();
            return Token(Token::TokenType::PIPE, inputBuffer->getLine(), inputBuffer->getColumn());
        }
    } else if (ch == '^') {
        if (peek() == '=') {
            forward();
            commitLexeme();
            return Token(Token::TokenType::CARET_ASSIGNMENT, inputBuffer->getLine(), inputBuffer->getColumn());
        } else {
            commitLexeme();
            return Token(Token::TokenType::CARET, inputBuffer->getLine(), inputBuffer->getColumn());
        }
    } else if (ch == '~') {
        commitLexeme();
        return Token(Token::TokenType::TILDE, inputBuffer->getLine(), inputBuffer->getColumn());
    } else if (ch == '%') {
        if (peek() == '=') {
            commitLexeme();
            return Token(Token::TokenType::PERCENT_ASSIGNMENT, inputBuffer->getLine(), inputBuffer->getColumn());
        } else {
            commitLexeme();
            return Token(Token::TokenType::PERCENT, inputBuffer->getLine(), inputBuffer->getColumn());
        }
    } else {
        THROW_LEXICAL_ERROR("Invalid operator");
    }
}

Lexer::Lexer(InputBuffer *inputBuffer, SymbolTable *symbolTable) {
    this->inputBuffer = inputBuffer;
    this->symbolTable = symbolTable;
}