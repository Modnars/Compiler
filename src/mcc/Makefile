parse.out : lex.yy.c symbol.h
	gcc -Wall lex.yy.c -o ../bin/parse.out -ll

lex.yy.c : token.lex symbol.h
	lex token.lex

run :
	cat ../test/hello_world.c | ../bin/parse.out
