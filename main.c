#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>


typedef struct {
    int x;
    int y;
} Vec2;

void compute_position(Vec2 window_size, Vec2 logo_size, Vec2* logo_position, Vec2* logo_velocity) {
    if (logo_position->x <= 0) logo_velocity->x *=-1;
    if (logo_position->y <= 0) logo_velocity->y *=-1;
    if (logo_position->x + logo_size.x >= window_size.x-1) logo_velocity->x *= -1;
    if (logo_position->y + logo_size.y >= window_size.y-1) logo_velocity->y *= -1;
    
    logo_position->x += logo_velocity->x;
    logo_position->y += logo_velocity->y;
}

void draw(SDL_Renderer* renderer, SDL_Texture* texture, Vec2 logo_size, Vec2 logo_position) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    SDL_Rect rectangle;
    rectangle.x = logo_position.x;
    rectangle.y = logo_position.y;
    rectangle.w = logo_size.x;
    rectangle.h = logo_size.y;
    SDL_RenderCopy(renderer, texture, NULL, &rectangle);
}

void quit_sdl(int code, SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture) {
    if (texture != NULL) SDL_DestroyTexture(texture); 
    IMG_Quit();
    if (renderer != NULL) SDL_DestroyRenderer(renderer);
    if (window != NULL) SDL_DestroyWindow(window);
    SDL_Quit();
    
    if (code != EXIT_SUCCESS)
        exit(code);
}

int main() {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;
    
    Vec2 window_size;
    window_size.x = 1080;
    window_size.y = 720;
    Vec2 logo_size;
    logo_size.x = 512;
    logo_size.y = 261;
    Vec2 logo_position;
    logo_position.x = 1;
    logo_position.y = 1;
    Vec2 logo_velocity;
    logo_velocity.x = 2;
    logo_velocity.y = 2;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
        quit_sdl(EXIT_FAILURE, window, renderer, texture);
    }
    
    window = SDL_CreateWindow(
        "Test de fenêtre",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_size.x, window_size.y,
        SDL_WINDOW_RESIZABLE
    );
    
    if (window == NULL) {
        SDL_Log("Error : SDL window creation - %s\n", SDL_GetError());
        quit_sdl(EXIT_FAILURE, window, renderer, texture);
    }
    
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    
    if (renderer == NULL) {
        SDL_Log("Error : SDL renderer creation - %s\n", SDL_GetError());
        quit_sdl(EXIT_FAILURE, window, renderer, texture);
    }
    
    int flags = IMG_INIT_PNG;
    if ((IMG_Init(flags) & flags) != flags) {
        SDL_Log("Error : SDL image loader\n");
        quit_sdl(EXIT_FAILURE, window, renderer, texture);
    }
    
    SDL_Surface* surface = IMG_Load("DVD_logo.png");
    if (surface == NULL) {
        SDL_Log("Error : image loading failed - %s\n", IMG_GetError());
        quit_sdl(EXIT_FAILURE, window, renderer, texture);
    }
    
    
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL) {
        SDL_Log("Error : texture loading failed\n");
        quit_sdl(EXIT_FAILURE, window, renderer, texture);
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
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    window_size.x = event.window.data1;
                    window_size.y = event.window.data2;
                }
                break;
            default:
                break;
            }
        }
        
        compute_position(window_size, logo_size, &logo_position, &logo_velocity);
        draw(renderer, texture, logo_size, logo_position);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    
    
    quit_sdl(EXIT_SUCCESS, window, renderer, texture);
    return EXIT_SUCCESS;
}
