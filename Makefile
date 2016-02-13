#variables
COMPILE = g++
FLAGS = -Wall -Werror -ansi -pedantic

#targets
all: clean
	mkdir ./bin
	$(COMPILE) $(FLAGS) ./src/rshell.cpp -o ./bin/rshell

rshell: clean
	mkdir ./bin
	$(COMPILE) $(FLAGS) ./src/rshell.cpp -o ./bin/rshell
	
clean:
	rm -rf ./bin