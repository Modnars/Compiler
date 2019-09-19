// Name   : scanner.hpp
// Author : Modnar
// Date   : 2019-05-18
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef __SCANNER_HPP
#define __SCANNER_HPP

#include <iostream>
#include <string>
#include <map>
#include <memory>

#include "Compiler.hpp"

class Token;
class Scanner;
extern int scan(std::ostream &os = std::cout);
// TODO Make the overload of print function is friendly.
extern std::ostream &operator<<(std::ostream &, const Token &);
extern bool numeric(const Token&);
extern Token max(const Token&, const Token&);

/* Mark the basic data type. */
extern const Token Int, Real, Char, Bool;

/* Mark the basic reserved words. */
extern const Token And, Or, eq, ne, le, ge, minus, True, False, temp;

class Token {
public:
    /* Constructor of Token. */
    Token(Mark tot) : tot(tot), width(0), ival(0) { }

    Token(int ival)         : tot(Mark::CINT),  width(0), ival(ival) { }
    Token(bool bval)        : tot(Mark::CBOOL), width(0), bval(bval) { }
    Token(char cval)        : tot(Mark::CCHAR), width(0), cval(cval) { }
    Token(double dval)      : tot(Mark::CREAL), width(0), dval(dval) { }
    Token(std::string sval) : tot(Mark::CSTR),  width(0), sval(sval) { }

    // TODO Add the checkRep for the below two constructors! In case of destructor 
    // could not recovery the memory!
    Token(std::string lexeme, Mark mark) : tot(mark), width(0), lexeme(lexeme) { }
    Token(std::string s, Mark m, int w): tot(m), width(w), lexeme(s) { }

    /* Copy Constructor. */
    Token(const Token &tk) : tot(tk.tot), width(tk.width) { copyUnion(tk); }
    Token &operator=(const Token&);

    /* Destructor of Token. */
    ~Token() { if (tot == Mark::CSTR) sval.~basic_string(); 
               else if (isLexeme()) lexeme.~basic_string(); }

    /* Methods to get token information. */
    int getTot() const;
    std::string getLexeme() const;
    int getWidth() const;
    bool isLexeme() const;

private:
    Mark tot; 
    int width; // For make the token to contains Type.
    union {
        int ival;
        bool bval;
        char cval;
        double dval;
        std::string sval;
        std::string lexeme;
    };
    void copyUnion(const Token&);
};

/* Scanner is used to scan the source file, and get the recognized tokens. */
class Scanner {
public:
    static int line;
    char peek = ' ';
    std::shared_ptr<std::map<std::string, Token>> words;

    // Check the token whether is lexeme.
    void reserve(Token tk);

    Scanner() {
    }
};

#endif /* __SCANNER_HPP */
