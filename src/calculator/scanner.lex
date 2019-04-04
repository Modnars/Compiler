%{
    #define YYSTYPE double
    #include <stdlib.h>
    #include "parser.h"
    void yyerror(char *);
%}

%%

[0-9]+          {
                    yylval = atof(yytext);
                    return NUMBER;
                }
[0-9]*\.[0-9]+  {
                    yylval = atof(yytext);
                    return NUMBER;
                }
[-+()*/\n]          return *yytext;
[ \t]           { /* Skip the space char */ }
.               yyerror("Invalid character.");

%%

int yywrap() {
    return 1;
}
