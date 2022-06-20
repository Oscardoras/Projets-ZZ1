#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#define SNAKE_COUNT 10
struct snake
{
	unsigned int x = 0;
	unsigned int y = 0;
}

void draw(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect rectangle;
	rectangle.x = 0;
  	rectangle.y = 0;
  	rectangle.w = 800;
  	rectangle.h = 600;
	SDL_RenderFillRect(renderer, &rectangle);
}
void eventWhile(SDL_Renderer* renderer)
{
	SDL_Event event;
	int continuer = 1;
	while (continuer) {
  		if(SDL_PollEvent(&event)) {
    		switch (event.type) {
    			case SDL_QUIT:
      				continuer = 0;
      			break;
		      }
		draw(renderer);
        	SDL_RenderPresent(renderer);
  		SDL_Delay(10);
		}
 	}
}
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
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	eventWhile(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
