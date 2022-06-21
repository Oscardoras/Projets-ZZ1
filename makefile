gameoflifemake: main.c viewport.o world_update.o
	gcc -g -Wall -Wextra main.c world.c viewport.o world_update.o -o gameoflife -lSDL2
viewport.o: viewport.h viewport.c
	gcc viewport.c viewport.h -c  viewport.o
world_update.o: world_update.h world_update.c
	gcc world_update.c world_update.h -c world_update.o
clean:
	rm -r *.o
