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

int scan(std::ostream &os) {
    os << "Scanning...\nDone." << std::endl;
    return EXIT_SUCCESS;
}

int Scanner::line = 1;

void Scanner::increase() {
    line += 1;
}
