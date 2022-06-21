gameoflifemake: main.c viewport.o world.o world_update.o
	gcc -g -Wall -Wextra main.c viewport.o world.o world_update.o -o gameoflife -lSDL2
viewport.o: viewport.h viewport.c
	gcc -c viewport.c -o viewport.o
world.o: world.h world.c
	gcc -c world.c -o world.o
world_update.o: world_update.h world_update.c
	gcc -c world_update.c -o world_update.o
clean:
	rm -r *.o
