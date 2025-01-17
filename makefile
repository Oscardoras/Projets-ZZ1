all: src/main.c bin/ai.o bin/entity.o bin/gameplay.o bin/level.o bin/markov.o bin/viewport.o
	gcc -g -Wall -Wextra src/main.c bin/ai.o bin/entity.o bin/level.o bin/markov.o bin/viewport.o bin/gameplay.o -o bin/program -lSDL2 -lSDL2_image -lm

bin/ai.o: src/ai.h src/ai.c
	gcc -g -Wall -Wextra -c src/ai.c -o bin/ai.o
bin/entity.o: src/entity.h src/entity.c
	gcc -g -Wall -Wextra -c src/entity.c -o bin/entity.o
bin/gameplay.o: src/gameplay.h src/gameplay.c
	gcc -g -Wall -Wextra -c src/gameplay.c -o bin/gameplay.o
bin/level.o: src/level.h src/level.c
	gcc -g -Wall -Wextra -c src/level.c -o bin/level.o
bin/markov.o: src/markov.h src/markov.c
	gcc -g -Wall -Wextra -c src/markov.c -o bin/markov.o
bin/viewport.o: src/viewport.h src/viewport.c
	gcc -g -Wall -Wextra -c src/viewport.c -o bin/viewport.o

release:
	gcc -O3 src/main.c src/ai.c src/entity.c src/level.c src/markov.c src/viewport.c src/gameplay.c -o bin/program -lSDL2 -lSDL2_image -lm

clean:
	rm -r bin/*.o bin/program
