gameoflifemake: main.c viewport.o
	gcc -g -Wall -Wextra main.c world.c viewport.o -o gameoflife -lSDL2
viewport.o: viewport.h viewport.c
	gcc viewport.c viewport.h -c  viewport.o
