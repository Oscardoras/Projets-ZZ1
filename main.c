#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#define RAYON 0.6
#define WINDOWS_COUNT 200
#define ROTATION_PER_TICK 1
void rotate (SDL_Window** windows, unsigned int count, float * rotations, float rayon)
{
	for(unsigned int it = 0; it < count; ++it)
	{
		rotations[it] += ROTATION_PER_TICK;
		SDL_SetWindowPosition(windows[it], cos(rotations[it] * rayon), sin(rotations[it] * rayon));
	}
}
int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
        {
                SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
                exit(EXIT_FAILURE);
        }
	SDL_Window* windows[WINDOWS_COUNT];
	float rotations[WINDOWS_COUNT];
	for(unsigned int it = 0; it < WINDOWS_COUNT; ++it)
	{
		rotations[it] = 0;
	}
	unsigned int openedWindowsCount = 0;
	unsigned int begin = SDL_GetTicks();
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	unsigned int Width = DM.w;
	unsigned int Height = DM.h;
	while(SDL_GetTicks() - begin <= 1000000)
	{
		SDL_Log("Error : SDL initialisation - %d\n", SDL_GetTicks() - begin);
		if(openedWindowsCount <= (SDL_GetTicks() - begin) / 1000)
		{
			++openedWindowsCount;
			windows[openedWindowsCount-1] = SDL_CreateWindow(
                	"Fenêtre à gauche",
                	0, 0,    
                	100, 10,
                	SDL_WINDOW_RESIZABLE);
			}
			if(windows[openedWindowsCount-1] == NULL)
        		{
                	SDL_Log("Error : SDL window 1 creation - %s\n)",
                        SDL_GetError());
                	SDL_Quit();   
                	exit(EXIT_FAILURE);
        	}
		SDL_Delay(100);
		rotate(windows, openedWindowsCount, rotations, Height * RAYON);
	}
}
