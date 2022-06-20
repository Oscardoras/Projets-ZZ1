#include <SDL2/SDL.h>
#include <stdio.h>


int main() {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    SDL_Window* window = SDL_CreateWindow(
        "Test de fenêtre",
        0, 0,
        400, 300,
        SDL_WINDOW_RESIZABLE
    );
    
    if (window == NULL) {
        SDL_Log("Error : SDL window creation - %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    SDL_Delay(2000);
    
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
