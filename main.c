#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define TEXTURE_NAME "sprites/FourmiGuerriere.png"
void eventWhile(SDL_Renderer* renderer, SDL_Texture *texture)
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
		}
        	SDL_RenderPresent(renderer);
  		SDL_Delay(60);
 	}
}

int main(int argc, char**argv)
{
    (void) argc;
	(void) argv;
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
        {
                SDL_Log("erreur initialisation SDL\n");
                exit(EXIT_FAILURE);
        }
	SDL_Window* window =
    SDL_CreateWindow(
                	"animation",
                	0, 0,
                	37, 12, SDL_WINDOW_OPENGL);
    if (window == NULL)
	{
		SDL_Log("Erreur creation fenetre");
		exit(EXIT_FAILURE);
	}
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
	{
		SDL_Log("Erreur creation renderer");
		exit(EXIT_FAILURE);
	}
	SDL_Texture* texture; 
  	texture = IMG_LoadTexture(renderer,TEXTURE_NAME);
  	if (texture == NULL)
	{
		SDL_Log("Erreur creation texture");
		exit(EXIT_FAILURE);
	} 
    eventWhile(renderer, texture);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
	SDL_Quit();
    return 0;
}