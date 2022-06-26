debug: src/main.c
	gcc -g -Wall -Wextra src/main.c -o bin/program -lSDL2 -lSDL2_image

release:
	gcc -O3 src/main.c -o bin/program -lSDL2 -lSDL2_image

clean:
	rm -r bin/*.o bin/program
