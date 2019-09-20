CC         =javac
CFLAGS     =-d
DRIVER     =java
PROGRAM    =main.Main
TARGET_DIR =../bin/

all :
	$(CC) $(CFLAGS) $(TARGET_FOLDER) lexer/*.java
	$(CC) $(CFLAGS) $(TARGET_FOLDER) symbols/*.java
	$(CC) $(CFLAGS) $(TARGET_FOLDER) inter/*.java
	$(CC) $(CFLAGS) $(TARGET_FOLDER) parser/*.java
	$(CC) $(CFLAGS) $(TARGET_FOLDER) main/*.java

run :
	cd $(TARGET_DIR) && $(DRIVER) $(PROGRAM)

test :
	cd $(TARGET_DIR); cat ../file/test.cpp | $(DRIVER) $(PROGRAM)
