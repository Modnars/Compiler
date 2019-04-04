/**
 * Author : Modnar
 * Date   : 2019-03-23
 * Copyright (C) 2019 Modnar. All rights reserved.
 */
#ifndef SYMBOL_H
#define SYMBOL_H

#include <string.h>

const int KEYWORD_NUM = 32;

const int AUTO        = 1;
const int BREAK       = 2;
const int CASE        = 3;
const int CHAR        = 4;
const int CONST       = 5;
const int CONTINUE    = 6;
const int DEFAULT     = 7;
const int DO          = 8;
const int DOUBLE      = 9;
const int ELSE        = 10;
const int ENUM        = 11;
const int EXTERN      = 12;
const int FLOAT       = 13;
const int FOR         = 14;
const int GOTO        = 15;
const int IF          = 16; // { return (IF); }
const int INT         = 17;
const int LONG        = 18;
const int REGISTER    = 19;
const int RETURN      = 20;
const int SHORT       = 21;
const int SIGNED      = 22;
const int SIZEOF      = 23;
const int STATIC      = 24;
const int STRUCT      = 25;
const int SWITCH      = 26;
const int TYPEDEF     = 27;
const int UNION       = 28;
const int UNSIGNED    = 29;
const int VOID        = 30;
const int VOLATILE    = 31;
const int WHILE       = 32;

const char *keyword_table[KEYWORD_NUM] = 
        {"auto", "break", "case", "char", "const", "continue", "default", "do",
         "double", "else", "enum", "extern", "float", "for", "goto", "if",
         "int", "long", "register", "return", "short", "signed", "sizeof", "static",
         "struct", "switch", "typedef", "union", "unsigned", "void", "volatile",
         "while"};

extern int search_key(const char *id) {
    for (size_t i = 0; i < KEYWORD_NUM; ++i)
        if (strcmp(keyword_table[i], id) == 0)
            return i+1;
    return 0;
}

#endif /* SYMBOL_H */
