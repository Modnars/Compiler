CC            =g++
CFLAGS        =-std=c++11 -I$(HEADFILES_DIR) -Wall
OBJS          =GrammarAnalysis.o GrammarParser.o Test.o main.o
SRCS          =../src/GrammarAnalysis.cpp ../src/GrammarParser.cpp Test.cpp main.cpp
HEADFILES_DIR =../include/
PROGRAM       =main.out

all : $(OBJS) $(HEADFILES_DIR)*.hpp
	$(CC) $(CFLAGS) $(OBJS) -o $(PROGRAM)

$(OBJS) : $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -c

clean :
	rm -rf *.o *.out
