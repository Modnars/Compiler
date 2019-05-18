// Name   : scanner.hpp
// Author : Modnar
// Date   : 2019-05-18
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

#include "Base.hpp"

class Token {
public:
    Token() : type(CINT), ival{0} { }
    Token(const Token &t) : type(t.type) { copyUnion(t); }
    Token &operator=(const Token&);

    ~Token() { if (type == CSTRING) sval.~basic_string(); }

    Token &operator=(int);
    Token &operator=(bool);
    Token &operator=(double);
    Token &operator=(const std::string &);

private:
    enum { CINT, CBOOL, CREAL, CSTRING } type;
    union {
        int ival;
        bool bval;
        double dval;
        std::string sval;
    };
    void copyUnion(const Token &);
};

#endif /* TOKEN_HPP */
