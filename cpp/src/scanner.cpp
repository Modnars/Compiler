// Name   : scanner.hpp
// Author : Modnar
// Date   : 2019-05-21
// Copyright (C) 2019 Modnar. All rights reserved.

#include <cstdlib>

#include "scanner.hpp"

typedef std::shared_ptr<Token> token_ptr;

/* Several special constraint token. */
std::shared_ptr<Token> Int   = std::make_shared<Token>("int", Mark::INT, 4), 
                       Real  = std::make_shared<Token>("real", Mark::REAL, 8),
                       Char  = std::make_shared<Token>("char", Mark::CHAR, 1), 
                       Bool  = std::make_shared<Token>("bool", Mark::BOOL, 1);

std::shared_ptr<Token> And   = std::make_shared<Token>("&&", Mark::AND),
                       Or    = std::make_shared<Token>("||", Mark::OR),
                       True  = std::make_shared<Token>("true", Mark::TRUE),    
                       False = std::make_shared<Token>("false", Mark::FALSE), 
                       eq    = std::make_shared<Token>("==", Mark::EQ), 
                       ne    = std::make_shared<Token>("!=", Mark::NE), 
                       le    = std::make_shared<Token>("<=", Mark::LE), 
                       ge    = std::make_shared<Token>(">=", Mark::GE),
                       minus = std::make_shared<Token>("minus", Mark::MINUS), 
                       temp  = std::make_shared<Token>("t", Mark::TEMP);

int Scanner::line = 1;

void Scanner::reserve(std::shared_ptr<Token> tk) {
    (*words)[tk->getLexeme()] = tk;
}

int scan(std::ostream &os) {
    os << "Scanning...\nDone." << std::endl;
    return EXIT_SUCCESS;
}

