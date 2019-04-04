%{
    #define YYSTYPE double
    #include <stdio.h>
    int yylex();
    void yyerror(char *);
%}

%token NUMBER
%left '+''-'
%left '*''/'

%%

program: 
        program expr '\n'       { printf("%lg\n", $2); }
        |
        ;

expr:
        NUMBER                  { $$ = $1; }
        | expr '+' expr         { $$ = $1 + $3; }
        | expr '-' expr         { $$ = $1 - $3; }
        | expr '*' expr         { $$ = $1 * $3; }
        | expr '/' expr         { $$ = $1 / $3; }
        | '(' expr ')'          { $$ = $2; }
        ;

%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

int main() {
    yyparse();
    return 0;
}
