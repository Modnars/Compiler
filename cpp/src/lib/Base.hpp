// Name   : Base.hpp
// Author : Modnar
// Date   : 2019-05-18
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef BASE_HPP
#define BASE_HPP

// Define Basic type.
typedef enum {
    /* Data type */
    VOID, INT, REAL, BOOL, STRING, STRUCT, ARRAY, 
    /* Reserved word */
    FOR, DO, WHILE, CONTINUE, BREAK, IF, ELSE, RETURN,
    /* Opearate symbol */
    PLUS, MINUS, MULTI, DEVIDE, LAND, LOR, LNOT, GT, GE, LT, LE, EQ, NE,
    INC, RED, AND, OR,
    /* Mark symbol */
    COMMA, SEMI, ASSIGN, DOT, QUOTE, 
    LPARTH, RPARTH, LBRACK, RBRACK, LBRACE, RBRACE,
    /* Value symbol */
    ID, CINT, CREAL, CBOOL, CSTRING,
    /* Control symbol */
    UNKNOWN, ERROR, NONE, ENDFILE
} BASIC_TYPE;

typedef union {
    int ival;
    bool bval;
    char cval;
    double dval;
} Value;

#endif /* BASE_HPP */
