#The C compiler to use.
CC=gcc
#The flags to use while compiling. Use the command 'man gcc' if you're
#unsure about what these flags do.
CFLAGS=-Wall -pedantic-errors -std=c99
#The name of the executable file to be generated.
EXEC=hello
#The name of the source code to be compiled.
SRC=hello.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)
clean: 
	-rm -rf *.o *~ "#"*"#" $(EXEC)
 
