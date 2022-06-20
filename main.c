#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

int main (int argc, char **argv)
{
	(void) argc;
	(void) argv;
	SDL_Window* window;
	SDL_Renderer* renderer;
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("Erreur initialisation SDL");
		exit(EXIT_FAILURE);
	}
	window = SDL_CreateWindow("Pavé de serpents",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, 800,
                            600,
                            SDL_WINDOW_OPENGL);


}
