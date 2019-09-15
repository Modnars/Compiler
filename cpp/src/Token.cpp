// Name   : scanner.hpp
// Author : Modnar
// Date   : 2019-05-18
// Copyright (C) 2019 Modnar. All rights reserved.

#include <memory>

#include "include/Token.hpp"

Token &Token::operator=(int i) {
    if (type == CSTRING) sval.~basic_string();
    ival = i;
    type = CINT;
    return *this;
}

Token &Token::operator=(bool b) {
    if (type == CSTRING) sval.~basic_string();
    bval = b;
    type = CBOOL;
    return *this;
}

Token &Token::operator=(double d) {
    if (type == CSTRING) sval.~basic_string();
    dval = d;
    type = CREAL;
    return *this;
}

Token &Token::operator=(const std::string &s) {
    if (type == CSTRING) {
        sval = s;
    } else {
        new (&sval) std::string(s);
        type = CSTRING;
    }
    return *this;
}

void Token::copyUnion(const Token &t) {
    switch (t.type) {
        case CINT : ival = t.ival; break;
        case CBOOL : bval = t.bval; break;
        case CREAL : dval = t.dval; break;
        case CSTRING : new (&sval) std::string(t.sval); break;
        default : break;
    }
}

Token &Token::operator=(const Token &t) {
    if (type == CSTRING && t.type != CSTRING) sval.~basic_string();
    if (type == CSTRING && t.type == CSTRING) sval = t.sval;
    else copyUnion(t);
    type = t.type;
    recognized = t.recognized;
    return *this;
}
