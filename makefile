game: src/main.c bin/viewport.o bin/level.o bin/character.o bin/matrix.o
	gcc -g -Wall -Wextra src/main.c bin/viewport.o bin/level.o bin/character.o bin/matrix.o -o game -lSDL2

bin/viewport.o: src/viewport.h src/viewport.c
	gcc -g -Wall -Wextra -c src/viewport.c -o bin/viewport.o
bin/level.o: src/level.h src/level.c
	gcc -g -Wall -Wextra -c src/level.c -o bin/level.o
bin/character.o: src/character.h src/character.c
	gcc -g -Wall -Wextra -c src/character.c -o bin/character.o
bin/matrix.o: src/matrix.h src/matrix.c
	gcc -g -Wall -Wextra -c src/matrix.c -o bin/matrix.o

clean:
	rm -r bin/*
