// Name   : Base.hpp
// Author : Modnar
// Date   : 2019-09-18
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef __COMPILER_HPP
#define __COMPILER_HPP

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

#endif /* __COMPILER_HPP */
