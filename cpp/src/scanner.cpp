// Name   : scanner.hpp
// Author : Modnar
// Date   : 2019-05-21
// Copyright (C) 2019 Modnar. All rights reserved.

#include <cstdlib>
#include <cctype>

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

std::shared_ptr<Token> Scanner::scan(std::istream &is) {
    for ( ; ; readch(is)) {
        if (peek == ' ' || peek == '\t') continue;
        else if (peek == '\n') ++line;
        else break;
    }
    switch (peek) {
        case '&':
            if (readch('&', is)) return And; 
            else return make_token("&", Mark::TK);
        case '|':
            if (readch('|', is)) return Or;
            else return make_token("|", Mark::TK);
        case '=':
            if (readch('=', is)) return eq;
            else return make_token("=", Mark::TK);
        case '!':
            if (readch('=', is)) return ne;
            else return make_token("!", Mark::TK);
        case '<':
            if (readch('=', is)) return le;
            else return make_token("<", Mark::TK);
        case '>':
            if (readch('=', is)) return ge;
            else return make_token(">", Mark::TK);
    }
    if (isdigit(peek)) {
        int ival = 0;
        do {
            ival = 10 * ival + (peek - '0'); readch(is);
        } while (isdigit(peek));
        if (peek != '.') return make_token(ival);
        double dval = ival, base = 10.0; 
        for ( ; ; ) {
            readch(is);
            if (!isdigit(peek)) break;
            dval = dval + (peek - '0') / base; 
            base *= 10.0;
        }
        return make_token(dval);
    }
    if (isalpha(peek) || peek == '_') {
        std::string str;
        do {
            str += peek; readch(is);
        } while (isalpha(peek) || isdigit(peek) || peek == '_');
        auto iter = words->find(str);
        if (iter != words->end()) return iter->second;
        auto id = make_token(str, Mark::ID);
        (*words)[str] = id;
        return id;
    }
    char ch = peek; peek = ' ';
    return make_token(std::string(1, ch), Mark::TK);
}

/* Class Scanner's private member functions. */
void Scanner::readch(std::istream &is) {
    peek = is.get();
}

bool Scanner::readch(char ch, std::istream &is) {
    readch(is);
    if (peek != ch) return false;
    peek = ' ';
    return true;
}
