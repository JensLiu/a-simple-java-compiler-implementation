//
// Created by jens on 30/05/23.
//

#ifndef COMPILER_INPUTBUFFER_H
#define COMPILER_INPUTBUFFER_H


#include <string>
#include <fstream>

class InputBuffer {
public:
    static int const BUFFER_SIZE = 1024;
private:
    // index arithmetics
    static int const HALF_BUFFER_SIZE = BUFFER_SIZE;
    static int const FULL_BUFFER_SIZE = 2 * BUFFER_SIZE + 2; // 2 extra eof signs at the end of each half to act like sentinels
    static int const FIRST_HALF_HEAD = 0;
    static int const FIRST_HALF_TAIL = HALF_BUFFER_SIZE - 1;
    static int const FIRST_HALF_SENTINEL = HALF_BUFFER_SIZE;
    static const int SECOND_HALF_HEAD = FIRST_HALF_SENTINEL + 1;
    static int const SECOND_HALF_TAIL = FULL_BUFFER_SIZE - 2;
    static int const SECOND_HALF_SENTINEL = FULL_BUFFER_SIZE - 1;

    // line and column number of the current character
    int line{};
    int column{};

    char buffer[FULL_BUFFER_SIZE]{};    // NOTE: {} performs a zero initialisation of the buffer

    int current = -1;   // index of the current character in the buffer

    std::string filename;
    std::fstream fin;

private:
    void loadFirstHalf();
    void loadSecondHalf();

public:
    explicit InputBuffer(const std::string& filename);
    ~InputBuffer();

    char getChar();
    void next();
    char getNextChar();
    char peek();

    int getLine() const;
    int getColumn() const;

    std::string getFilename() const;

    void printBuffer() const;

};


#endif //COMPILER_INPUTBUFFER_H
