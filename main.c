#include <SDL2/SDL.h>
#include <stdio.h>


int main() {
    int window_size = 300, window_x = 0, window_y = 0, mouse_x, mouse_y;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    SDL_Window* window = SDL_CreateWindow(
        "Test de fenêtre",
        window_x, window_y,
        window_size, window_size,
        SDL_WINDOW_RESIZABLE
    );
    
    if (window == NULL) {
        SDL_Log("Error : SDL window creation - %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    
    SDL_bool running = SDL_TRUE;
    SDL_Event event;
    
    while (running) {
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = SDL_FALSE;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = SDL_FALSE;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT))
                    window_size = 600;
                else
                    window_size = 300;
                break;
            default:
                break;
            }
            
            SDL_SetWindowSize(window, window_size, window_size);
            SDL_GetWindowPosition(window, &window_x, &window_y);
            SDL_GetMouseState(&mouse_x, &mouse_y);
            SDL_SetWindowPosition(window, window_x + mouse_x-(window_size/2), window_y + mouse_y-(window_size/2));
        }
        
        SDL_Delay(10);
    }
    
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
