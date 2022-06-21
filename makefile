game: src/main.c
	gcc -g -Wall -Wextra src/main.c -o game -lSDL2
clean:
	rm -r bin/*
