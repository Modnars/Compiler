// Name   : Compiler.cpp
// Author : Modnar
// Date   : 2019-09-20
// Copyright (C) 2019 Modnar. All rights reserved.

#include "Compiler.hpp"

/* Definition of class Token. */
/**
 * The Assign-Constructor of class Token.
 */
Token &Token::operator=(const Token &tk) {
    if (mark == Mark::CSTR && tk.mark != Mark::CSTR) {
        sval.~basic_string();
    }
    if (isLexeme() && !tk.isLexeme()) {
        lexeme.~basic_string();
    }
    if (mark == Mark::CSTR && tk.mark == Mark::CSTR) {
        sval = tk.sval;
    } else if (isLexeme() && tk.isLexeme()) {
        lexeme = tk.lexeme;
    } else {
        copyUnion(tk);
    }
    mark = tk.mark;
    width = tk.width;
    return *this;
}

/**
 * Get a token's mark.
 * @return the token's mark.
 */
Mark Token::getMark() const {
    return mark;
}

/**
 * Get a token's lexeme value.
 * @return the token's lexeme.
 */
std::string Token::getLexeme() const {
    if (isLexeme()) {
        return lexeme;
    }
    return "";
}

/**
 * When the token object is regarded as a specific type, get the 
 * type's size (with bytes).
 * @return the type's size.
 */
int Token::getWidth() const {
    return width;
}

/**
 * Judge whether the token is a lexeme through the token's mark.
 * The token of lexeme contains lexeme value.
 * @return true if the token is a lexeme, otherwise return false.
 */
bool Token::isLexeme() const {
    // TODO Whether should be implemented with 'switch'? Waiting for test.
    if (mark == Mark::AND || mark == Mark::OR || mark == Mark::EQ || mark == Mark::NE ||
        mark == Mark::LE  || mark == Mark::GE || mark == Mark::TRUE || 
        mark == Mark::FALSE || mark == Mark::MINUS || mark == Mark::TEMP || 
        mark == Mark::INT || mark == Mark::REAL)
        return true;
    return false;
}

/* Token's private function. */
void Token::copyUnion(const Token &tk) {
    switch (tk.mark) {
        case Mark::CINT : ival = tk.ival; break;
        case Mark::CBOOL: bval = tk.bval; break;
        case Mark::CCHAR: cval = tk.cval; break;
        case Mark::CREAL: dval = tk.dval; break;
        case Mark::CSTR : new(&sval) std::string(tk.sval); break;
        default: break;
    }
    if (tk.isLexeme()) new(&lexeme) std::string(tk.lexeme);
}

std::ostream &operator<<(std::ostream &os, const Token &tok) {
    os << "Tk:" << (int)tok.getMark() << " " << tok.getLexeme();
    return os;
}

