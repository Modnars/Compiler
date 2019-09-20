// Name   : scanner.hpp
// Author : Modnar
// Date   : 2019-05-18
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef __SCANNER_HPP
#define __SCANNER_HPP

#include "Compiler.hpp"

#include <map>
#include <memory>

class Scanner;
extern int scan(std::ostream &os = std::cout);
// extern std::shared_ptr<Token> recognize();

/* Mark the basic data type. */
extern const Token Int, Real, Char, Bool;

/* Mark the basic reserved words. */
extern const Token And, Or, True, False, eq, ne, le, ge, minus, temp;

/* Scanner is used to scan the source file, and get the recognized tokens. */
class Scanner {
public:
    static int line;
    char peek = ' ';
    std::shared_ptr<std::map<std::string, Token>> words;

    // Check the token whether is lexeme.
    // void reserve(Token tk);
    void increase(); 

    Scanner() { }
};

#endif /* __SCANNER_HPP */
