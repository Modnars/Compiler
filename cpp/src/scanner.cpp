// Name   : scanner.hpp
// Author : Modnar
// Date   : 2019-05-21
// Copyright (C) 2019 Modnar. All rights reserved.

#include <cstdlib>

#include "scanner.hpp"

/* Several special constraint token. */
const Token Int("int",   Mark::INT,  4);
const Token Real("real", Mark::REAL, 8);
const Token Char("char", Mark::CHAR, 1);
const Token Bool("bool", Mark::BOOL, 1);

extern const Token And, Or, eq, ne, le, ge, minus, True, False, temp;
const Token And("&&", Mark::AND), Or("||", Mark::OR), eq("==", Mark::EQ), 
            ne("!=", Mark::NE), le("<=", Mark::LE), ge(">=", Mark::GE),
            minus("minus", Mark::MINUS), True("true", Mark::TRUE), 
            False("false", Mark::FALSE), temp("t", Mark::TEMP);

int scan(std::ostream &os) {
    os << "Scanning...\nDone." << std::endl;
    return EXIT_SUCCESS;
}

std::ostream &operator<<(std::ostream &os, const Token &tok) {
    os << (int)tok.tot;
    return os;
}

bool isLexeme(Mark m) {
    if (m == Mark::AND || m == Mark::OR || m == Mark::EQ || m == Mark::NE ||
        m == Mark::LE  || m == Mark::GE || m == Mark::TRUE || m == Mark::FALSE ||
        m == Mark::MINUS || m == Mark::TEMP || m == Mark::INT || m == Mark::REAL)
        return true;
    return false;
}

