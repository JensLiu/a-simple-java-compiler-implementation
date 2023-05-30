//
// Created by jens on 30/05/23.
//

#include <iostream>
#include "InputBuffer.h"

InputBuffer::InputBuffer(const std::string &filename) {
    this->filename = filename;

    this->fin.open(filename);
    if (!fin.is_open()) {
        throw std::runtime_error("Could not open file " + filename);
    }

    // buffer is initialised to 0's before
    // set sentinels
    this->buffer[FIRST_HALF_SENTINEL] = EOF;
    this->buffer[SECOND_HALF_SENTINEL] = EOF;

    // load first half
    loadFirstHalf();
    loadSecondHalf();

}

InputBuffer::~InputBuffer() {
    this->fin.close();
}

void InputBuffer::loadFirstHalf() {
    char ch;
    for (int i = FIRST_HALF_HEAD; i <= FIRST_HALF_TAIL; i++) {
        if (fin.get(ch)) {
            buffer[i] = ch;
        } else {
            buffer[i] = EOF;
            break;
        }
    }
}

void InputBuffer::loadSecondHalf() {
    char ch;
    for (int i = SECOND_HALF_HEAD; i <= SECOND_HALF_TAIL; i++) {
        if (fin.get(ch)) {
            buffer[i] = ch;
        } else {
            buffer[i] = EOF;
            break;
        }
    }
}

void InputBuffer::printBuffer() const {
    for (char i: buffer) {
        if (i == EOF) {
            std::cout << "$";
        } else if (i == '\n') {
            std::cout << "\\n";
        } else if (i == '\t') {
            std::cout << "\\t";
        } else if (i == '\r') {
            std::cout << "\\r";
        } else if (i == '\0') {
            std::cout << "\\0";
        } else {
            std::cout << i;
        }
    }
    std::cout << std::endl;
}

int InputBuffer::getLine() const {
    return this->line;
}

int InputBuffer::getColumn() const {
    return this->column;
}

std::string InputBuffer::getFilename() const {
    return this->filename;
}

char InputBuffer::getChar() {
    return buffer[current];
}

void InputBuffer::next() {
    current++;
    if (buffer[current] == EOF) {
        if (current == FIRST_HALF_SENTINEL) {
//            std::cout << "[debug]: reloading second half" << std::endl;
//            printBuffer();
//            std::cout << std::endl;
            loadFirstHalf();
            current = SECOND_HALF_HEAD;
        } else if (current == SECOND_HALF_SENTINEL) {
//            std::cout << "[debug]: reloading first half" << std::endl;
//            printBuffer();
//            std::cout << std::endl;
            loadSecondHalf();
            current = FIRST_HALF_HEAD;
        }
        // else, EOF as character not as sentinel
    }

    // update line and column
    if (buffer[current] == '\n') {
        line++;
        column = 0;
    } else {
        column++;
    }
}


char InputBuffer::getNextChar() {
    next();
    return getChar();
}

char InputBuffer::peek() {
    if (current + 1 == FIRST_HALF_SENTINEL) {
        return buffer[SECOND_HALF_HEAD];
    } else if (current + 1 == SECOND_HALF_SENTINEL) {
        return buffer[FIRST_HALF_HEAD];
    }
    return buffer[current + 1];

}
