CC          =g++
CFLAGS      =-std=c++11 -Wall -I ../include/
OBJ         =GrammarAnalysis.o GrammarParser.o SLR.o LR.o main.o
SRC         =GrammarAnalysis.cpp GrammarParser.cpp SLR.cpp LR.cpp main.cpp
HEADFILES   =../include/*.hpp
PROGRAM     =main
DEBUGER     =lldb

#t : 
#	g++ -std=c++11 -Wall -I ../include/ GrammarAnalysis.cpp GrammarParser.cpp SLR.cpp LR.cpp main.cpp

all : $(PROGRAM)

$(PROGRAM) : $(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) -o $(PROGRAM).out

$(OBJ) : $(SRC) $(HEADFILES)
	$(CC) $(CFLAGS) $(SRC) -c

d :
	$(CC) $(CFLAGS) $(SRC) -o $(PROGRAM).dbg -g
	$(DEBUGER) $(PROGRAM).dbg

clean :
	rm -rf *.o *.out *.dbg *.dbg.dSYM $(PROGRAM)
