#include <iostream>
#include "InputBuffer.h"
#include "Lexer.h"
#include "ContextFreeGrammar.h"
#include "SymbolTable.h"
#include "Parser.h"
#include "grammar_def.h"

extern std::vector<Production> grammarDefs;

using std::cout;
using std::endl;
using std::string;

void inputBufferTest();

void lexerTest();

void grammarTest();

void parserTest();
void leftRecursionEliminationTest();

int main() {
    leftRecursionEliminationTest();
//    lexerTest();
//    grammarTest();
//    parserTest();
    return 0;
}

void parserTest() {
    ContextFreeGrammar grammar(grammarDefs);
    InputBuffer inputBuffer("../test/parser_test_expression");
    SymbolTable symbolTable;
    Lexer lexer(&inputBuffer, &symbolTable);
    Parser parser(grammar, &lexer, &symbolTable);
    parser.parse();
}

void leftRecursionEliminationTest() {
    ContextFreeGrammar grammar({
        Production(HEAD("<expr>"), {NT("<expr>"), T(Token::PLUS), NT("<term>")}),
        Production(HEAD("<expr>"), {NT("<expr>"), T(Token::MINUS), NT("<term>")}),
        Production(HEAD("<expr>"), {NT("<term>")}),
        Production(HEAD("<term>"), {NT("<term>"), T(Token::STAR), NT("<factor>")}),
        Production(HEAD("<term>"), {NT("<term>"), T(Token::SLASH), NT("<factor>")}),
        Production(HEAD("<term>"), {NT("<factor>")}),
        Production(HEAD("<factor>"), {T(Token::LEFT_PAREN), NT("<expr>"), T(Token::RIGHT_PAREN)}),
        Production(HEAD("<factor>"), {T(Token::INTEGER_LITERAL)}),
    });
    cout << "before elimination" << endl;
    grammar.printProductions();
    cout << endl << "after elimination" << endl;
    grammar.eliminateDirectLeftRecursive();
    grammar.printProductions();
}

void grammarTest() {
    // dangling else
    ContextFreeGrammar nearestElse(
            {
                    // S ::= F
                    Production(HEAD("S"), {NT("F")}),
                    // S ::= F else S
                    Production(HEAD("S"), {NT("F"), T(Token::ELSE), NT("S")}),
                    // F ::= if ( E ) S
                    Production(HEAD("F"), {T(Token::IF), T(Token::LEFT_PAREN), NT("E"), T(Token::RIGHT_PAREN)}),
                    // E ::= Int
                    Production(HEAD("E"), {T(Token::INTEGER_LITERAL)}),
            });
    ContextFreeGrammar farthestElse(
            {
                    // S ::= F else S
                    Production(HEAD("S"), {NT("F"), T(Token::ELSE), NT("S")}),
                    // S ::= F
                    Production(HEAD("S"), {NT("F")}),
                    // F ::= if ( E ) S
                    Production(HEAD("F"), {T(Token::IF), T(Token::LEFT_PAREN), NT("E"), T(Token::RIGHT_PAREN)}),
                    // E ::= Int
                    Production(HEAD("E"), {T(Token::INTEGER_LITERAL)}),
            });
    std::cout << "nearest else" << endl;
    nearestElse.printParsingTable();
    cout << endl << "farthest else" << endl;
    farthestElse.printParsingTable();
}


void inputBufferTest() {
    InputBuffer inputBuffer("../test/lexer_test_java_programme");
    char ch;
    while ((ch = inputBuffer.getChar()) != EOF) {
        inputBuffer.next();
        cout << ch;
    }
    cout << "$";

}

void lexerTestDriver(const std::string &description, const std::string &pathname) {
    cout << description << endl << "BEGIN" << endl;
    InputBuffer inputBuffer(pathname);
    SymbolTable symbolTable;
    Lexer lexer(&inputBuffer, &symbolTable);
    for (int i = 0;; i++) {
        Token token = lexer.nextToken();
        if (token.isWhitespace())
            continue;
        cout << "\t" << token << endl;
        if (token.getTokenType() == Token::END_OF_FILE) {
            break;
        }
    }
    cout << "END" << endl;
}

void lexerTest() {
    lexerTestDriver("this test should tokenlise the string literal with escape characters correctly",
                    "../test/lexer_test_escape_sequence");
    try {
        lexerTestDriver("this test should detect the error on 1e-", "../test/lexer_test_error_report");
    } catch (LexicalError &e) {
        cout << "\tError caught:\t" << e.what() << endl << "END" << endl;
    }
    lexerTestDriver("this test should tokenlise the java programme correctly",
                    "../test/lexer_test_java_programme");
}
