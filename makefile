all: src/main.c bin/entity.o bin/level.o bin/matrix.o bin/viewport.o
	gcc -g -Wall -Wextra src/main.c bin/entity.o bin/level.o bin/matrix.o bin/viewport.o -o game -lSDL2

bin/entity.o: src/entity.h src/entity.c
	gcc -g -Wall -Wextra -c src/entity.c -o bin/entity.o
bin/level.o: src/level.h src/level.c
	gcc -g -Wall -Wextra -c src/level.c -o bin/level.o
bin/matrix.o: src/matrix.h src/matrix.c
	gcc -g -Wall -Wextra -c src/matrix.c -o bin/matrix.o
bin/viewport.o: src/viewport.h src/viewport.c
	gcc -g -Wall -Wextra -c src/viewport.c -o bin/viewport.o

clean:
	rm -r bin/*
	rm game
