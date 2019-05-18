/**
 * Author : Modnar
 * Date   : 2019-03-23
 * Copyright (C) 2019 Modnar. All rights reserved.
 */

%{
#include <stdio.h>
#include "symbol.h"
    /* definitions of manifest constants
    LT, LE, EQ, NE, GT, GE,
    IF, THEN, ELSE, ID, NUMBER, RELOP */
//digit       [0-9]+
//float       [0-9]*\.[0-9]+
//{digit}     printf("[DIGIT   : %-8s]\n", yytext);
//{float}     printf("[FLOAT   : %-8s]\n", yytext);
%}

/* regular definitions */
delim       [ \t]
ws          {delim}+
letter      [A-Za-z]
id          ([_a-zA-Z])([_a-zA-Z0-9])*
string      \".+\"
head_file   \<[A-Za-z]+\.h\>
operator    [\+\-\*\/]

%%

[0-9]*\.[0-9]+  printf("Here Double\n");
[0-9]+          printf("Here Digit\n");
{ws}        { /* no action and no return */ }
\n          printf("\n");
#include    printf("[PRE_PROCESS       ]\n");
{head_file} printf("[HEADFILE: %-8s]\n", yytext);
{id}        { 
                int id;
                if ((id = search_key(yytext)) != 0)
                    printf("[Keyword : %-8s]\n", yytext); 
                else
                    printf("[ID      : %-8s]\n", yytext); 
                // { yylval = (int) installID(); return (ID); }
            }

{operator}  printf("[OPERATOR: %-8s]\n", yytext);

"++"        printf("[INC     : %-8s]\n", yytext);

"="         printf("[ASSIGN            ]\n");
"<"         printf("[LESS_THAN         ]\n"); // { yylval = LT; return (RELOP); }
"<="        printf("[LESS_OR_EQUAL     ]\n"); // { yylval = LE; return (RELOP); }
">"         printf("[GREAT_THAN        ]\n"); // { yylval = GT; return (RELOP); }
">="        printf("[GREAT_OR_EQUAL    ]\n"); // { yylval = GE; return (RELOP); }
"=="        printf("[EQUAL             ]\n"); // { yylval = EQ; return (RELOP); }
"!="        printf("[NOT_EQUAL         ]\n"); // { yylval = NE; return (RELOP); }
"("         printf("[LEFT_SMALL_BRACE  ]\n");
")"         printf("[RIGHT_SMALL_BRACE ]\n");
"["         printf("[LEFT_MID_BRACE    ]\n");
"]"         printf("[RIGHT_MID_BRACE   ]\n");
"{"         printf("[LEFT_BIG_BRACE    ]\n");
"}"         printf("[RIGHT_BIG_BRACE   ]\n");
";"         printf("[SEMICOLON         ]\n");
","         printf("[COMMA             ]\n");
{string}    printf("[STRING            ]\n");

%%
