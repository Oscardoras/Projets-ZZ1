#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define TEXTURE_NAME "sprites/FourmiGuerriere.png"
#define WINDOW_SIZE_X 370
#define WINDOW_SIZE_Y 120
SDL_Rect rects[3];
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
			SDL_Rect destination;
			destination.x = 0;
			destination.y = 0;
			destination.w = WINDOW_SIZE_X;
			destination.h = WINDOW_SIZE_Y;
			SDL_RenderCopy(renderer, texture,
                 &rects[time(0)%3],
                 &destination);
        	SDL_RenderPresent(renderer);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderClear(renderer);
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
                	WINDOW_SIZE_X, WINDOW_SIZE_Y, SDL_WINDOW_OPENGL);
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
	rects[0].x = 0;
	rects[0].y = 0;
	rects[0].w = 37;
	rects[0].h = 12;
	rects[1].x = 0;
	rects[1].y = 12*3;
	rects[1].w = 37;
	rects[1].h = 12;
	rects[2].x = 0;
	rects[2].y = 12*4;
	rects[2].w = 37;
	rects[2].h = 12;
    eventWhile(renderer, texture);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
	SDL_Quit();
    return 0;
}