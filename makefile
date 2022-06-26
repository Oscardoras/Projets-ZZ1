debug: src/main.c bin/viewport.o bin/world.o
	gcc -g -Wall -Wextra src/main.c bin/viewport.o bin/world.o -o bin/program -lSDL2
bin/viewport.o: src/viewport.h src/viewport.c
	gcc -g -Wall -Wextra -c src/viewport.c -o bin/viewport.o
bin/world.o: src/world.h src/world.c
	gcc -g -Wall -Wextra -c src/world.c -o bin/world.o

release:
	gcc -O3 src/main.c src/viewport.c src/world.c -o bin/program -lSDL2

clean:
	rm -r bin/*.o bin/program
