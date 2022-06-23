#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


int main() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Erreur init - %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
    }
    
    SDL_Window* window = SDL_CreateWindow("Anim",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_RESIZABLE
    );
    
    if(!window) {
        SDL_Log("Error Window - %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    
    SDL_Texture* texture1 = NULL;
    SDL_Texture* texture2 = NULL;
    SDL_Texture* texture3 = NULL;
    
    SDL_Texture* tab_textures[3] = {texture1, texture2, texture3};
}