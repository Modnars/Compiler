LEX=lex
LEXFLAGS= 
YACC=bison
YACCFLAGS=-vtd
CC=gcc
CFLAGS=
LIBS=
PARSER=parser
SCANNER=scanner
OBJS=$(PARSER).o $(SCANNER).o
PROGRAM=calculator 

all : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROGRAM)

$(PARSER).o : $(PARSER).c $(PARSER).h
	$(CC) $(CFLAGS) $(PARSER).c -c

$(SCANNER).o : $(SCANNER).c $(PARSER).h
	$(CC) $(CFLAGS) $(SCANNER).c -c

$(SCANNER).c : $(SCANNER).lex
	$(LEX) $(LEXFLAGS) -o $(SCANNER).c $(SCANNER).lex 

$(PARSER).c : $(PARSER).yacc
	$(YACC) $(YACCFLAGS) $(PARSER).yacc -o $(PARSER).c

run :
	./$(PROGRAM)

clean :
	rm *.o $(SCANNER).c $(PARSER).c $(PARSER).h $(PARSER).output $(PROGRAM)
