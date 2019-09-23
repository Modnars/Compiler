CC          =g++
CFLAGS      =-std=c++11 -Wall -I ../include/
PARSER      =parsers
MAIN        =main
OBJ         =$(PARSER).o BasicFunc.o Item.o Production.o 
SRC         =$(PARSER).cpp BasicFunc.cpp Item.cpp Production.cpp
HEADFILES   =../include/*.hpp
PROGRAM     =result
DEBUGER     =lldb

t : 
	g++ -std=c++11 -Wall -I ../include/ GrammaAnalysis.cpp main.cpp

all : $(PROGRAM)
	echo "Helel"

$(PROGRAM) : $(OBJ) $(MAIN).o
	echo "Helel"
	$(CC) $(CFLAGS) $(OBJ) $(MAIN).o -o $(PROGRAM)

$(OBJ) : $(SRC) $(HEADFILES)
	$(CC) $(CFLAGS) $(SRC) -c

$(MAIN).o : $(MAIN).cpp $(HEADFILES)
	echo "Helel"
	$(CC) $(CFLAGS) $(MAIN).cpp -c

d :
	$(CC) $(CFLAGS) $(SRC) -o $(MAIN).dbg -g
	$(DEBUGER) $(MAIN).dbg

r : 
	./$(PROGRAM) -n ../test/test.cpp

clean :
	rm -rf *.o *.out *.dbg *.dbg.dSYM $(PROGRAM)
