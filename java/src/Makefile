CC            =javac
CFLAGS        =-d
DRIVER        =java
PROGRAM       =main.Main
TARGET_FOLDER =../bin/

all :
	$(CC) $(CFLAGS) $(TARGET_FOLDER) lexer/*.java
	$(CC) $(CFLAGS) $(TARGET_FOLDER) symbols/*.java
	$(CC) $(CFLAGS) $(TARGET_FOLDER) inter/*.java
	$(CC) $(CFLAGS) $(TARGET_FOLDER) parser/*.java
	$(CC) $(CFLAGS) $(TARGET_FOLDER) main/*.java

run :
	cd $(TARGET_FOLDER) && $(DRIVER) $(PROGRAM)

test :
	cd $(TARGET_FOLDER); cat ../file/test.cpp | $(DRIVER) $(PROGRAM)
