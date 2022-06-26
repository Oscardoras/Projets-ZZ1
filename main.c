#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 600
#define WORLD_SIZE_X 1000 
#define WORLD_SIZE_Y 600
#define SPRITE_COUNT 10
typedef struct
{
	unsigned int count;
	SDL_Rect rects[10];
	unsigned int x;
	unsigned int y;
	unsigned int sizex;
	unsigned int sizey;
	unsigned int textureid;
} sprite;
sprite sprites[SPRITE_COUNT];
#define TEXTURE_COUNT 10
SDL_Texture* textures[TEXTURE_COUNT];
char* TEXTURE_NAMES[] = {"sprites/Free Pixel Art Forest/PNG/Background layers/Layer_0002_7.png", 
"sprites/Free Pixel Art Forest/PNG/Background layers/Layer_0011_0.png", 
"sprites/Free Pixel Art Forest/PNG/Background layers/Layer_0009_2.png",
"sprites/Free Pixel Art Forest/PNG/Background layers/Layer_0008_3.png",
"sprites/Free Pixel Art Forest/PNG/Background layers/Layer_0006_4.png",
"sprites/Free Pixel Art Forest/PNG/Background layers/Layer_0005_5.png",
"sprites/Free Pixel Art Forest/PNG/Background layers/Layer_0003_6.png",
"sprites/Free Pixel Art Forest/PNG/Background layers/Layer_0001_8.png",
"sprites/Free Pixel Art Forest/PNG/Background layers/Layer_0000_9.png",
"sprites/characters/player.png"
};
unsigned int POSX = 0;
unsigned int POSX_BACKGROUND = 0;
unsigned int POSX_BACKGROUND2 = 0;
unsigned int POSY = 500;
void eventWhile(SDL_Renderer* renderer)
{
	SDL_Event event;
	int continuer = 1;
	unsigned int timeout = 0;
	unsigned int timeout_background = 0;
	while (continuer) {
  		while(SDL_PollEvent(&event)) {
    		switch (event.type) {
    			case SDL_QUIT:
      				continuer = 0;
      			break;
		      }
			} 
			for(unsigned int it = 0; it < SPRITE_COUNT; ++it)
			{
				SDL_Rect destination;
				destination.x = sprites[it].x;
				destination.y = sprites[it].y;
				destination.w = sprites[it].sizex;
				destination.h = sprites[it].sizey;
				SDL_RenderCopy(renderer, textures[sprites[it].textureid],
                 &sprites[it].rects[time(0)%sprites[it].count],
                 &destination);
			}
        	SDL_RenderPresent(renderer);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderClear(renderer);
  			SDL_Delay(60);
			if(POSX < 400)
			{
				sprites[1].x = -POSX_BACKGROUND2;
				sprites[2].x = -POSX_BACKGROUND2;
				sprites[3].x = -POSX_BACKGROUND2;
				sprites[4].x = -POSX_BACKGROUND2;

				sprites[5].x = (-POSX_BACKGROUND);
				sprites[6].x = (-POSX_BACKGROUND);
				sprites[8].x = (-POSX_BACKGROUND);
				sprites[9].x = POSX;
				++timeout;
				timeout += SDL_GetTicks();
				if(timeout >800)
				{
					timeout = 0;
					++POSX;
					++POSX;
				}
				++timeout_background;
				timeout_background += SDL_GetTicks();
				if(timeout_background >16000)
				{
					timeout_background = 0;
					++POSX_BACKGROUND2;
					++POSX_BACKGROUND2;
					++POSX_BACKGROUND;
				}
			}
			else
			{
				sprites[9].count = 1;
				sprites[9].rects[0].x = 113;
				sprites[9].rects[0].y = 223;
				sprites[9].rects[0].w = 88-66;
				sprites[9].rects[0].h = 235-219;
				sprites[9].sizex = (88-66)*3;
				sprites[9].sizey = (235-219)*3;
				++timeout;
				timeout += SDL_GetTicks();
				if(timeout >800)
				{
					sprites[9].y += 20;
				}
			}
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
	for(unsigned int it = 0; it < TEXTURE_COUNT; ++it)
	{
		printf("%s", TEXTURE_NAMES[it]);
		textures[it] = IMG_LoadTexture(renderer,TEXTURE_NAMES[it]);
		if(!textures[it])
		{
			SDL_Log("Erreur creation texture");
			exit(EXIT_FAILURE);
		}
	}
	/**init sprites**/
	sprites[0].count = 1;
	sprites[0].x = 0;
	sprites[0].y = 0;
	sprites[0].sizex = WORLD_SIZE_X;
	sprites[0].sizey = WORLD_SIZE_Y;
	sprites[0].textureid = 1;
	sprites[0].rects[0].x = 0;
	sprites[0].rects[0].y = 0;
	sprites[0].rects[0].w = 928;
	sprites[0].rects[0].h = 793;

	sprites[1].count = 1;
	sprites[1].x = 0;
	sprites[1].y = 0;
	sprites[1].sizex = WORLD_SIZE_X;
	sprites[1].sizey = WORLD_SIZE_Y;
	sprites[1].textureid = 2;
	sprites[1].rects[0].x = 0;
	sprites[1].rects[0].y = 0;
	sprites[1].rects[0].w = 928;
	sprites[1].rects[0].h = 793;

	sprites[8].count = 1;
	sprites[8].x = 0;
	sprites[8].y = 0;
	sprites[8].sizex = WORLD_SIZE_X;
	sprites[8].sizey = WORLD_SIZE_Y;
	sprites[8].textureid = 0;
	sprites[8].rects[0].x = 0;
	sprites[8].rects[0].y = 0;
	sprites[8].rects[0].w = 928;
	sprites[8].rects[0].h = 793;

	sprites[2].count = 1;
	sprites[2].x = 0;
	sprites[2].y = 0;
	sprites[2].sizex = WORLD_SIZE_X;
	sprites[2].sizey = WORLD_SIZE_Y;
	sprites[2].textureid = 3;
	sprites[2].rects[0].x = 0;
	sprites[2].rects[0].y = 0;
	sprites[2].rects[0].w = 928;
	sprites[2].rects[0].h = 793;


	sprites[3].count = 1;
	sprites[3].x = 0;
	sprites[3].y = 0;
	sprites[3].sizex = WORLD_SIZE_X;
	sprites[3].sizey = WORLD_SIZE_Y;
	sprites[3].textureid = 4;
	sprites[3].rects[0].x = 0;
	sprites[3].rects[0].y = 0;
	sprites[3].rects[0].w = 928;
	sprites[3].rects[0].h = 793;

	sprites[4].count = 1;
	sprites[4].x = 0;
	sprites[4].y = 0;
	sprites[4].sizex = WORLD_SIZE_X;
	sprites[4].sizey = WORLD_SIZE_Y;
	sprites[4].textureid = 5;
	sprites[4].rects[0].x = 0;
	sprites[4].rects[0].y = 0;
	sprites[4].rects[0].w = 928;
	sprites[4].rects[0].h = 793;


	sprites[5].count = 1;
	sprites[5].x = 0;
	sprites[5].y = 0;
	sprites[5].sizex = WORLD_SIZE_X;
	sprites[5].sizey = WORLD_SIZE_Y;
	sprites[5].textureid = 6;
	sprites[5].rects[0].x = 0;
	sprites[5].rects[0].y = 0;
	sprites[5].rects[0].w = 928;
	sprites[5].rects[0].h = 793;

	sprites[6].count = 1;
	sprites[6].x = 0;
	sprites[6].y = 0;
	sprites[6].sizex = WORLD_SIZE_X;
	sprites[6].sizey = WORLD_SIZE_Y;
	sprites[6].textureid = 7;
	sprites[6].rects[0].x = 0;
	sprites[6].rects[0].y = 0;
	sprites[6].rects[0].w = 928;
	sprites[6].rects[0].h = 793;

	sprites[7].count = 1;
	sprites[7].x = 0;
	sprites[7].y = 0;
	sprites[7].sizex = WORLD_SIZE_X;
	sprites[7].sizey = WORLD_SIZE_Y;
	sprites[7].textureid = 8;
	sprites[7].rects[0].x = 0;
	sprites[7].rects[0].y = 0;
	sprites[7].rects[0].w = 928;
	sprites[7].rects[0].h = 793;


	sprites[9].count = 2;
	sprites[9].x = POSX;
	sprites[9].y = POSY;
	sprites[9].sizex = (32-17)*3;
	sprites[9].sizey = (90-68)*3;
	sprites[9].textureid = 9;
	sprites[9].rects[0].x = 17;
	sprites[9].rects[0].y = 68;
	sprites[9].rects[0].w = 32-17;
	sprites[9].rects[0].h = 90-68;
	sprites[9].rects[1].x = 65;
	sprites[9].rects[1].y = 70;
	sprites[9].rects[1].w = 32-17;
	sprites[9].rects[1].h = 90-68;
	/** fin init sprites**/
    eventWhile(renderer);
	SDL_DestroyWindow(window);
	for(unsigned int it = 0; it < TEXTURE_COUNT; ++it)
		SDL_DestroyTexture(textures[it]);
	SDL_Quit();
    return 0;
}