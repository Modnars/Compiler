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
extern std::shared_ptr<Token> Int, Real, Char, Bool;

/* Mark the basic reserved words. */
extern std::shared_ptr<Token> And, Or, True, False, eq, ne, le, ge, minus, temp;

/* Scanner is used to scan the source file, and get the recognized tokens. */
class Scanner {
public:
    static int line;
    char peek = ' ';
    std::shared_ptr<std::map<std::string, std::shared_ptr<Token>>> words = nullptr;

    // TODO Need to check whether the Token object is a lexeme.
    void reserve(std::shared_ptr<Token> tk);

    Scanner() { 
        words = std::make_shared<std::map<std::string, std::shared_ptr<Token>>>();
        reserve(std::make_shared<Token>("if", Mark::IF));
        reserve(std::make_shared<Token>("else", Mark::ELSE));
        reserve(std::make_shared<Token>("do", Mark::DO));
        reserve(std::make_shared<Token>("while", Mark::WHILE));
        reserve(std::make_shared<Token>("break", Mark::BREAK));
        reserve(True); reserve(False);
        reserve(Int);  reserve(Char);
        reserve(Bool); reserve(Real);
    }
};

#endif /* __SCANNER_HPP */
