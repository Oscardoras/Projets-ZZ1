all: src/main.c 
	gcc -g -Wall -Wextra main.c -o game -lSDL2
clean:
	rm -r bin/*
	rm game
 