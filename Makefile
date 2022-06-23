renderRect : renderRect.c
	gcc renderRect.c -o renderRect -Wall -Wextra -lSDL2 -lm
	
anim : anim.c
	gcc anim.c -o anim -Wall -Wextra -lSDL2 -lSDL2_image