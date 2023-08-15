# Documentation

Reference: Aho, Sethi, Ullman, *Compilers: Principles, Techniques, and Tools*, Addison-Wesley, 1986.

# Lexical analyser

The lexical analyser is implemented by hand, using conditions and loops. The state transform graph of the (hypothetical) automata is as follows

 

![Untitled-2023-05-20-1600.png](Documentation%readme/automata.png)

## Input Buffer

The input buffer is implemented using **two buffer scheme (buffer pair)** which divides the buffer into two parts, each with `EOF` at the end as sentinels. The implementation can be found in `InputBuffer.h` and `InputBuffer.cpp`.

### Construction

`InputBuffer(const std::string& filename)`

```cpp
InputBuffer inputBuffer("test_code")
```

### Features

It reads lines from a file and provides interface to access the characters and move forward.

- `next`: move the cursor to the next position
- `getChar`: get the character at the current position
- `getNextChar`: first move to the next position, then get the character at that new position.
- `peek`: get the character at the next position without moving the cursor.
- `getLine`: get the line number of the cursor in the file
- `getColumn`: get the column number of the cursor in the file

## Symbol Table

the `SymbolTable` is a dummy table that is implemented for the sake of maintaining the structure of the “compiler”. The implementation can be found in `SymbolTable.h` and `SymbolTable.cpp`.
It only supports the following operations:

- `addSymbol`: add the symbol string to the table, return the index in the symbol table
- `getSymbol`: get the symbol name at the provided index

## Lexer

The `Lexer` reads strings from `InputBuffer` and writes symbol details to `SymbolTable`. It output a `Token` each time. The implementation can be found in `Lexer.h` and `Lexer.cpp`.

### Construction

`Lexer(InputBuffer *inputBuffer, SymbolTable *symbolTable)`

```cpp
InputBuffer inputBuffer("test_code");
SymbolTable symbolTable;
Lexer lexer(&inputBuffer, &symbolTable);
```

### Features

- `nextToken`: get as token the next lexeme string from the file

### Exception

throws `LexicalError` when encountering un-parseable strings. It reports its position in the file.

### Example usage

```cpp
InputBuffer inputBuffer("your_test_code");
SymbolTable symbolTable;
Lexer lexer(&inputBuffer, &symbolTable);
for (int i = 0;; i++) {
		Token token = lexer.nextToken();
		if (token.isWhitespace())
			continue;
		cout << token << endl;
		if (token.getTokenType() == Token::END_OF_FILE) {
			break;
		}
}
```

# Syntax Analysis

The syntax analysis takes `Token` produced by `Parser`, and analyses it with `ContextFreeGrammar` (currently it only supports LL(1) grammar). LL(1) grammar is provided as a list of `Production`s to the `ContextFreeGrammar` to produce FIRST and FOLLOW tables to predict the next production to use.

The `Parser` will do the left-most derivation by maintaining a stack and pushing symbols in the body of the production in reverse order.

## Grammar Symbol

`GrammarSymbol` is a symbol in the LL(1) grammar. It can be either terminal or non-terminal. Terminal symbols are identified solely by their names, while non-terminal symbols adopt the structure of `Token`s.Its implementation can be found in `GrammarSymbol.h` and `GrammarSymbol.cpp`.

### Creation

- terminal `GrammarSymbol::createTerminal(Token::TokenType::YOUR_TOKEN_TYPE_ENUM)`
- non-terminal`GrammarSymbol::createNonTerminal("<symbol-name>")`
- epsilon `GrammarSymbol::epsilon()`
- end of file `GrammarSymbol::eof()`
- invalid symbol: it is only used in a zero-parameter constructor of a `Production` to indicate an internal error.

## LL(1) Grammar

Currently, the `ContextFreeGrammar` is exclusively implemented for LL(1) grammars. To construct it, provide a list of `Production`s. Its implementation is found in `ContextFreeGrammar.h` and `ContextFreeGrammar.cpp`.

### Features

- Eliminate direct left recursion
- Calculate FIRST and FOLLOW set
- Calculate parsing table (LL(1) prediction table)
- export parsing table (LL(1) prediction table) as csv

### Example Usage

`Production` creation macros: `HEAD` for the head of the production, `NT` for non-terminal, `T` for terminal.

```cpp
ContextFreeGrammar grammar(
          {
                Production(HEAD("E"), {NT("E"), T(Token::PLUS), NT("T")}),
                Production(HEAD("E"), {NT("E"), T(Token::MINUS), NT("T")}),
                Production(HEAD("E"), {NT("T")}),
                Production(HEAD("T"), {NT("T"), T(Token::STAR), NT("F")}),
                Production(HEAD("T"), {NT("T"), T(Token::SLASH), NT("F")}),
                Production(HEAD("T"), {NT("F")}),
                Production(HEAD("F"),
                           {T(Token::LEFT_PAREN), NT("E"), T(Token::RIGHT_PAREN)}),
                Production(HEAD("F"), {T(Token::IDENTIFIER)}),
          });
cout << "before elimination" << endl;
grammar.printProductions();
cout << endl << "after elimination" << endl;
grammar.eliminateDirectLeftRecursive().printProductions();
grammar.exportParsingTableAsCsv("../app_output/left_elimination_parsing_table.csv");
```

## Parser

Utilizing a `ContextFreeGrammar`, it processes every `Token` supplied by the `Lexer`, aided by the dummy `SymbolTable` (which serves a structural role within the "compiler" but does not contribute to actual functionality), in order to analyse the syntax of the provided code.

The implementation can be found in `Parser.h` and `Parser.cpp`

### Exception

It throws a `SyntacticalError` when encountering un-parseable symbol which does not comply with the grammar.

### Example Usage

```cpp
ContextFreeGrammar yourGrammar
InputBuffer inputBuffer("your_test_code");
SymbolTable symbolTable;
Lexer lexer(&inputBuffer, &symbolTable);
Parser parser(grammar, &lexer, &symbolTable);
parser.parse();
```