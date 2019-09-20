// Name   : Base.hpp
// Author : Modnar
// Date   : 2019-09-18
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef __COMPILER_HPP
#define __COMPILER_HPP

#include <iostream>
#include <string>

enum class Mark {
    /* Data type */
    VOID, INT, BOOL, CHAR, REAL, STR, CLASS, ARRAY, 
    /* Reserved word */
    FOR, DO, WHILE, CONTINUE, BREAK, IF, ELSE, RETURN,
    /* Opearate symbol */
    PLUS, MINUS, MULTI, DEVIDE, LAND, LOR, LNOT, GT, GE, LT, LE, EQ, NE,
    INC, RED, AND, OR,
    /* Mark symbol */
    COMMA, SEMI, ASSIGN, DOT, QUOTE, LPARTH, RPARTH, LBRACK, RBRACK, LBRACE, RBRACE,
    /* Value symbol */
    ID, CINT, CBOOL, CCHAR, CREAL, CSTR, TRUE, FALSE, TEMP,
    /* Control symbol */
    UNKNOWN, ERROR, NONE, ENDFILE
};

class Token;
// TODO Make the overload of print function is friendly.
extern std::ostream &operator<<(std::ostream &, const Token &);
extern bool numeric(const Token&);
extern Token max(const Token&, const Token&);

class Token {
public:
    /* Constructor of Token. */
    Token(Mark mark) : mark(mark), width(0), ival(0) { }

    Token(int ival)         : mark(Mark::CINT),  width(0), ival(ival) { }
    Token(bool bval)        : mark(Mark::CBOOL), width(0), bval(bval) { }
    Token(char cval)        : mark(Mark::CCHAR), width(0), cval(cval) { }
    Token(double dval)      : mark(Mark::CREAL), width(0), dval(dval) { }
    Token(std::string sval) : mark(Mark::CSTR),  width(0), sval(sval) { }

    // TODO Add the checkRep for the below two constructors! In case of destructor 
    // could not recovery the memory!
    Token(std::string lexeme, Mark mark) : mark(mark), width(0), lexeme(lexeme) { }
    Token(std::string s, Mark m, int w): mark(m), width(w), lexeme(s) { }

    /* Copy Constructor. */
    Token(const Token &tk) : mark(tk.mark), width(tk.width) { copyUnion(tk); }
    Token &operator=(const Token&);

    /* Destructor of Token. */
    ~Token() { if (mark == Mark::CSTR) sval.~basic_string(); 
               else if (isLexeme()) lexeme.~basic_string(); }

    /* Methods to get token information. */
    Mark getMark() const;
    std::string getLexeme() const;
    int getWidth() const;
    bool isLexeme() const;

private:
    Mark mark; // Mark of a specific token.
    int width; // Mark the Type Token's size.
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

#endif /* __COMPILER_HPP */
