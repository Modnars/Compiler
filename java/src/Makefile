JAVAC      =javac
CFLAGS     =-d ../bin/
DRIVER     =java
PROGRAM    =main.Main

all :
	$(JAVAC) ${CFLAGS} lexer/*.java
	$(JAVAC) ${CFLAGS} symbols/*.java
	$(JAVAC) ${CFLAGS} inter/*.java
	$(JAVAC) ${CFLAGS} parser/*.java
	$(JAVAC) ${CFLAGS} main/*.java

run :
	cd ../bin/ && $(DRIVER) $(PROGRAM)

test :
	cd ../bin/; cat ../file/test.cpp | $(DRIVER) $(PROGRAM)
