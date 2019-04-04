# Find the OS platform using the uname command.
Linux := $(findstring Linux, $(shell uname -s))
MacOS := $(findstring Darwin, $(shell uname -s))
Windows := $(findstring NT, $(shell uname -s))

# Specify what typing 'make' on its own will compile
default: asteroid_boi

# For Windows, add the .exe extension
ifdef Windows

asteroid_boi: asteroid_boi.c
	clang -std=c11 -Wall -pedantic -g $@.c display.c -lSDL2 -o $@.exe

test: asteroid_boi.c test.c
	clang -std=c11 -Wall -pedantic -g asteroid_boi.c test.c -o $@.exe

%: %.c
	clang -Dtest_$@ -std=c11 -Wall -pedantic -g $@.c -o $@.exe

# For Linux/MacOS, include the advanced debugging options
else

asteroid_boi: asteroid_boi.c
	clang -std=c11 -Wall -o asteroid_boi asteroid_boi.c -lSDL2

test: asteroid_boi.c test.c
	clang -std=c11 -Wall -pedantic -g asteroid_boi.c test.c -o $@ \
	    -fsanitize=undefined

%: %.c
	clang -Dtest_$@ -std=c11 -Wall -pedantic -g $@.c -o $@ \
	    -fsanitize=undefined

endif
