game: main.c 
	gcc -g -Wall -Wextra main.c -o game -lSDL2 -lSDL2_image
clean:
	rm -r *.o
	rm game
 