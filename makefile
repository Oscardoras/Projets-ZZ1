gameoflifemake: src/main.c bin/viewport.o bin/world.o
	gcc -g -Wall -Wextra src/main.c bin/viewport.o bin/world.o -o gameoflife -lSDL2
bin/viewport.o: src/viewport.h src/viewport.c
	gcc -g -Wall -Wextra -c src/viewport.c -o bin/viewport.o
bin/world.o: src/world.h src/world.c
	gcc -g -Wall -Wextra -c src/world.c -o bin/world.o
clean:
	rm -r bin/*
