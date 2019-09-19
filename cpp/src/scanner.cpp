// Name   : scanner.hpp
// Author : Modnar
// Date   : 2019-05-21
// Copyright (C) 2019 Modnar. All rights reserved.

#include <cstdlib>

#include "scanner.hpp"

/* Several special constraint token. */
const Token Int("int",   Mark::INT,  4), Real("real", Mark::REAL, 8),
            Char("char", Mark::CHAR, 1), Bool("bool", Mark::BOOL, 1);

const Token And("&&", Mark::AND), eq("==", Mark::EQ), ne("!=", Mark::NE), 
            Or("||", Mark::OR),   le("<=", Mark::LE), ge(">=", Mark::GE),
            True("true", Mark::TRUE),    False("false", Mark::FALSE), 
            minus("minus", Mark::MINUS), temp("t", Mark::TEMP);

Token &Token::operator=(const Token &tk) {
    if (tot == Mark::CSTR && tk.tot != Mark::CSTR) {
        sval.~basic_string();
    }
    if (isLexeme() && !tk.isLexeme()) {
        lexeme.~basic_string();
    }
    if (tot == Mark::CSTR && tk.tot == Mark::CSTR) {
        sval = tk.sval;
    } else if (isLexeme() && tk.isLexeme()) {
        lexeme = tk.lexeme;
    } else {
        copyUnion(tk);
    }
    tot = tk.tot;
    width = tk.width;
    return *this;
}

int Token::getTot() const {
    return (int)tot;
}

std::string Token::getLexeme() const {
    if (isLexeme()) {
        return lexeme;
    }
    return "";
}

int Token::getWidth() const {
    return width;
}

bool Token::isLexeme() const {
    // TODO Whether should be implemented with 'switch'? Waiting for test.
    if (tot == Mark::AND || tot == Mark::OR || tot == Mark::EQ || tot == Mark::NE ||
        tot == Mark::LE  || tot == Mark::GE || tot == Mark::TRUE || 
        tot == Mark::FALSE || tot == Mark::MINUS || tot == Mark::TEMP || 
        tot == Mark::INT || tot == Mark::REAL)
        return true;
    return false;
}

/* Token's private function. */
void Token::copyUnion(const Token &tk) {
    switch (tk.tot) {
        case Mark::CINT:  ival = tk.ival; break;
        case Mark::CBOOL: bval = tk.bval; break;
        case Mark::CCHAR: cval = tk.cval; break;
        case Mark::CREAL: dval = tk.dval; break;
        case Mark::CSTR: new(&sval) std::string(tk.sval); break;
        default: break;
    }
    if (tk.isLexeme()) new(&lexeme) std::string(tk.lexeme);
}

int scan(std::ostream &os) {
    os << "Scanning...\nDone." << std::endl;
    return EXIT_SUCCESS;
}

std::ostream &operator<<(std::ostream &os, const Token &tok) {
    os << "Tk:" << tok.getTot() << " " << tok.getLexeme();
    return os;
}

// bool isLexeme(Mark m) {
//     // TODO Whether should be implemented with 'switch'? Waiting for test.
//     if (m == Mark::AND || m == Mark::OR || m == Mark::EQ || m == Mark::NE ||
//         m == Mark::LE  || m == Mark::GE || m == Mark::TRUE || m == Mark::FALSE ||
//         m == Mark::MINUS || m == Mark::TEMP || m == Mark::INT || m == Mark::REAL)
//         return true;
//     return false;
// }

