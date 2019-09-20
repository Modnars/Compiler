// Name   : scanner.hpp
// Author : Modnar
// Date   : 2019-05-21
// Copyright (C) 2019 Modnar. All rights reserved.

#include <cstdlib>

#include "scanner.hpp"

/* Several special constraint token. */
std::shared_ptr<Token> Int   = std::make_shared<Token>("int", Mark::INT, 4), 
                       Real  = std::make_shared<Token>("real", Mark::REAL, 8),
                       Char  = std::make_shared<Token>("char", Mark::CHAR, 1), 
                       Bool  = std::make_shared<Token>("bool", Mark::BOOL, 1);

std::shared_ptr<Token> And   = std::make_shared<Token>("&&", Mark::LAND),
                       Or    = std::make_shared<Token>("||", Mark::LOR),
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

std::shared_ptr<Token> Scanner::scan(std::istream &is, std::ostream &os) {
    for ( ; ; readch(is)) {
        if (peek == ' ' || peek == '\t') continue;
        else if (peek == '\n') ++line;
        else break;
    }
    switch (peek) {
        case '&':
            if (readch('&', is)) return And; 
            else return std::make_shared<Token>(Mark::AND);
        case '|':
            if (readch('|', is)) return Or;
            else return std::make_shared<Token>(Mark::OR);
        case '=':
            if (readch('=', is)) return eq;
            else return std::make_shared<Token>(Mark::ASSIGN);
        case '!':
            if (readch('=', is)) return ne;
            else return std::make_shared<Token>(Mark::LNOT);
        case '<':
            if (readch('=', is)) return le;
            else return std::make_shared<Token>(Mark::LT);
        case '>':
            if (readch('=', is)) return ge;
            else return std::make_shared<Token>(Mark::GT);
    }
    return EXIT_SUCCESS;
}

/* Class Scanner's private member functions. */
void Scanner::readch(std::istream &is) {
    peek = is.peek();
}

bool Scanner::readch(char ch, std::istream &is) {
    readch(is);
    if (peek != ch) return false;
    peek = ' ';
    return true;
}
