
#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define SNAKE_COUNT 7
#define SNAKE_SIZE_MIN 10
#define SNAKE_SIZE_MAX 20
#define DISTANCE_PER_TICK 20
typedef struct snake
{
	unsigned int x;
	unsigned int y;
	unsigned int length;
	float angle;
	struct snake * next;
	struct snake * before;
	unsigned int X3;
	unsigned int X2;
	unsigned int X1;
} snake_t;

void updateSerpent(snake_t* snake_ptr)
{
	if(snake_ptr->next)
                updateSerpent(snake_ptr->next);
	if(snake_ptr->before)
	{
		snake_ptr->x = snake_ptr->before->x;
                snake_ptr->y = snake_ptr->before->y;
                snake_ptr->angle = snake_ptr->before->angle;
	}
	else
	{
		snake_ptr->angle = (int)(pow(snake_ptr->X3*((SDL_GetTicks()/100)%100), 3) + pow(snake_ptr->X2*((SDL_GetTicks()/100)%100), 2) + pow(snake_ptr->X1*((SDL_GetTicks()/100)%100), 1))%360;
		snake_ptr->x = snake_ptr->x + cos(snake_ptr->angle) * DISTANCE_PER_TICK;
		snake_ptr->y = snake_ptr->y + sin(snake_ptr->angle) * DISTANCE_PER_TICK;
	}
	snake_ptr->x = snake_ptr->x%800;
	snake_ptr->y = snake_ptr->y%800;
}
void creationSerpent(snake_t** snake_ptr)
{
	*snake_ptr = malloc(sizeof(snake_t));
	if(*snake_ptr)
	{
		(*snake_ptr)->x = rand()%800;
		(*snake_ptr)->y = rand()%600;
		(*snake_ptr)->length = rand()%(SNAKE_SIZE_MAX - SNAKE_SIZE_MIN)+SNAKE_SIZE_MIN;
		(*snake_ptr)->angle = (float)(rand()%360) / 360 * ((2.0*3.14));
		(*snake_ptr)->before = 0;
		(*snake_ptr)->X3 = rand()%10;
                (*snake_ptr)->X2 = rand()%10;
		(*snake_ptr)->X1 = rand()%10;
	}
	else
		exit(EXIT_FAILURE);
	snake_t *cour = *snake_ptr;
	for(unsigned int it = 0; it < (*snake_ptr)->length; ++it)
	{
		cour->next = malloc(sizeof(snake_t));
		cour->next->before = cour;
		cour = cour->next;
		if(cour)
		{
			cour->x = 0;
                	cour->y = 0;
                	cour->length = 0;
                	cour->angle = 0;
		}
		else
			exit(EXIT_FAILURE);
		if(it+1 == (*snake_ptr)->length)
			cour->next = 0;
	}
}
void drawSnake(snake_t * snake_ptr, SDL_Renderer * renderer)
{
        if(snake_ptr->next)
                drawSnake(snake_ptr->next, renderer);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0,255);
	float angle4 = (snake_ptr->angle + 90.0) / 360.0 * 6.28;
	float angle3 = (snake_ptr->angle - 10.0) / 360.0 * 6.28;
	float angle1 = (snake_ptr->angle + 180-10.0) / 360.0 * 6.28;
        float angle2 = (snake_ptr->angle + 180+90.0) / 360.0 * 6.28;
	SDL_RenderDrawLine(renderer,10*cos(angle1) + snake_ptr->x, 10*sin(angle1) + snake_ptr->y, 10*cos(angle2) + snake_ptr->x, 10*sin(angle2) + snake_ptr->y);
	SDL_RenderDrawLine(renderer,10*cos(angle2) + snake_ptr->x, 10*sin(angle2) + snake_ptr->y, 10*cos(angle3) + snake_ptr->x, 10*sin(angle3) + snake_ptr->y);
	SDL_RenderDrawLine(renderer,10*cos(angle3) + snake_ptr->x, 10*sin(angle3) + snake_ptr->y, 10*cos(angle4) + snake_ptr->x, 10*sin(angle4) + snake_ptr->y);
	SDL_RenderDrawLine(renderer,10*cos(angle4) + snake_ptr->x, 10*sin(angle4) + snake_ptr->y, 10*cos(angle1) + snake_ptr->x, 10*sin(angle1) + snake_ptr->y);
}
void draw(SDL_Renderer* renderer, snake_t ** snakes) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect rectangle;
	rectangle.x = 0;
  	rectangle.y = 0;
  	rectangle.w = 800;
  	rectangle.h = 600;
	SDL_RenderFillRect(renderer, &rectangle);
	for(unsigned int it = 0; it < SNAKE_COUNT; ++it)
	{
		drawSnake(snakes[it], renderer);
	}
}
void eventWhile(SDL_Renderer* renderer)
{
	snake_t * snakes [SNAKE_COUNT];
	for(unsigned int it = 0; it < SNAKE_COUNT; ++it)
		creationSerpent(&snakes[it]);
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
		draw(renderer, snakes);
        	SDL_RenderPresent(renderer);
  		SDL_Delay(20);
		for(unsigned int it = 0; it < SNAKE_COUNT; ++it)
			updateSerpent(snakes[it]);
 	}
}
int main (int argc, char **argv)
{
	(void) argc;
	(void) argv;
	srand(time(0));
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
