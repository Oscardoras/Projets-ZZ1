#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#define RAYON 0.6
#define WINDOWS_COUNT 100
#define ROTATION_PER_TICK 0.2
#define DURATION 10000
void rotate (SDL_Window** windows, unsigned int count, float * rotations, unsigned int width, unsigned int height)
{
	for(unsigned int it = 0; it < count; ++it)
	{
		rotations[it] += ROTATION_PER_TICK;
		SDL_SetWindowPosition(windows[it], (width/2.0)+(cos(rotations[it]) * (float)width * RAYON), (height/2)+(sin(rotations[it]) * (float)height * RAYON));
	}
}
int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
        {
                SDL_Log("erreur initialisation SDL\n");
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
	while(SDL_GetTicks() - begin <= DURATION)
	{
		if(openedWindowsCount <= (SDL_GetTicks() - begin) / 1000 && openedWindowsCount < WINDOWS_COUNT)
		{
			++openedWindowsCount;
			windows[openedWindowsCount-1] = SDL_CreateWindow(
                	"Rotation",
                	0, 0,
                	100, 100,
                	SDL_WINDOW_RESIZABLE);
			}
			if(windows[openedWindowsCount-1] == NULL)
        		{
                	SDL_Log("Erreur creation de fenetre\n");
                	SDL_Quit();
                	exit(EXIT_FAILURE);
        	}
		SDL_Delay(100);
		rotate(windows, openedWindowsCount, rotations, Width, Height);
	}
	for(unsigned int it = 0; it < openedWindowsCount; ++it)
		SDL_DestroyWindow(windows[it]);
	SDL_Quit();
}
