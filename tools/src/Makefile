CC          =g++
CFLAGS      =-std=c++11 -Wall -I ../include/
OBJ         =GrammarAnalysis.o GrammarParser.o LL.o SLR.o LR.o main.o
SRC         =GrammarAnalysis.cpp GrammarParser.cpp LL.cpp SLR.cpp LR.cpp main.cpp
HEADFILES   =../include/*.hpp
PROGRAM     =analyze
DEBUGER     =lldb

all : $(PROGRAM)

$(PROGRAM) : $(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) -o $(PROGRAM)

$(OBJ) : $(SRC) $(HEADFILES)
	$(CC) $(CFLAGS) $(SRC) -c

d :
	$(CC) $(CFLAGS) $(SRC) -o $(PROGRAM).dbg -g
	$(DEBUGER) $(PROGRAM).dbg

clean :
	rm -rf *.o *.out *.dbg *.dbg.dSYM $(PROGRAM)
