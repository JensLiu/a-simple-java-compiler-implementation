#include <iostream>
#include "InputBuffer.h"
#include "Lexer.h"

using std::cout;
using std::endl;

void inputBufferTest();

void lexerTest();

int main() {
    lexerTest();
    return 0;
}

void inputBufferTest() {
    InputBuffer inputBuffer("/home/jens/Documents/tmp/test.java");
    char ch;
//    while ((ch = inputBuffer.getNextChar()) != EOF) {
//        cout << "current char: '" << ch << "'" << endl;
//        cout << "current buffer: " << endl;
//        inputBuffer.printBuffer();
//        cout << "--------------------------------" << endl;
//    }

    while ((ch = inputBuffer.getChar()) != EOF) {
        inputBuffer.next();
        cout << ch;
    }
    cout << "$";

}

void lexerTest() {
    InputBuffer inputBuffer("/home/jens/Documents/tmp/test.java");
    Lexer lexer(&inputBuffer);
    for (int i = 0;; i++) {
        Token token = lexer.nextToken();
//        if (token.getTokenType() != Token::WHITESPACE) {
        cout << "token: <" << token.getTokenType() << ", " << token.getLexeme() << ">" << endl;
//        }

        if (token.getTokenType() == Token::END_OF_FILE) {
            break;
        }
    }
}
