CC          =g++
CFLAGS      =-std=c++11 -Wall -I ../include/
MAIN        =main
OBJS        =
HEADFILES   =../include/*.hpp
PROGRAM     =result
DEBUGER     =lldb

all : $(PROGRAM)

$(PROGRAM) : $(OBJS) $(MAIN).o
	$(CC) $(CFLAGS) $(OBJS) $(MAIN).o -o $(PROGRAM)

$(MAIN).o : $(MAIN).cpp $(HEADFILES)
	$(CC) $(CFLAGS) $(MAIN).cpp -c

clean :
	rm -rf *.o $(PROGRAM)
