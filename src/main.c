#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdbool.h>
#include <stdio.h>

#define LEVELS 2


typedef struct {
    int velocity;
    struct {
        SDL_Rect rectangle;
        SDL_Texture* texture;
    } objects[10];
    int objects_size;
} Level;


void compute_position(Level *levels, int width) {
    for (int i = 0; i < LEVELS; i++) {
        for (int j = 0; j < levels[i].objects_size; j++) {
            levels[i].objects[j].rectangle.x += levels[i].velocity;

            if (levels[i].objects[j].rectangle.x > width)
                levels[i].objects[j].rectangle.x = - levels[i].objects[j].rectangle.w;
        }
    }
}

void draw(SDL_Renderer* renderer, Level *levels) {
    SDL_SetRenderDrawColor(renderer, 64, 255, 255, 128);
    SDL_RenderClear(renderer);

    for (int i = 0; i < LEVELS; i++) {
        for (int j = 0; j < levels[i].objects_size; j++) {
            SDL_RenderCopy(renderer, levels[i].objects[j].texture, NULL, &levels[i].objects[j].rectangle);
        }
    }

    SDL_RenderPresent(renderer);
}

void quit_sdl(SDL_Window* window, SDL_Renderer* renderer, Level *levels) {
    for (int i = 0; i < LEVELS; i++)
        for (int j = 0; j < levels[i].objects_size; j++)
            SDL_DestroyTexture(levels[i].objects[j].texture);
    IMG_Quit();

    if (renderer != NULL) SDL_DestroyRenderer(renderer);
    if (window != NULL) SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    Level levels[LEVELS];

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
        quit_sdl(window, renderer, levels);
        return EXIT_FAILURE;
    }
    
    window = SDL_CreateWindow(
        "Animation",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720,
        0
    );
    
    if (window == NULL) {
        SDL_Log("Error : SDL window creation - %s\n", SDL_GetError());
        quit_sdl(window, renderer, levels);
        return EXIT_FAILURE;
    }
    
    renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    
    if (renderer == NULL) {
        SDL_Log("Error : SDL renderer creation - %s\n", SDL_GetError());
        quit_sdl(window, renderer, levels);
        return EXIT_FAILURE;
    }
    
    
    levels[0].velocity = 1;
    levels[0].objects_size = 2;
    levels[0].objects[0].rectangle.x = -300;
    levels[0].objects[0].rectangle.y = 400;
    levels[0].objects[0].rectangle.w = 960;
    levels[0].objects[0].rectangle.h = 480;
    levels[0].objects[0].texture = IMG_LoadTexture(renderer, "textures/hill2.png");
    levels[0].objects[1].rectangle.x = 700;
    levels[0].objects[1].rectangle.y = 410;
    levels[0].objects[1].rectangle.w = 960;
    levels[0].objects[1].rectangle.h = 480;
    levels[0].objects[1].texture = IMG_LoadTexture(renderer, "textures/hill2.png");

    levels[1].velocity = 5;
    levels[1].objects_size = 4;
    levels[1].objects[0].rectangle.x = -350;
    levels[1].objects[0].rectangle.y = 510;
    levels[1].objects[0].rectangle.w = 1400;
    levels[1].objects[0].rectangle.h = 300;
    levels[1].objects[0].texture = IMG_LoadTexture(renderer, "textures/hill1.png");
    levels[1].objects[1].rectangle.x = 1000;
    levels[1].objects[1].rectangle.y = 500;
    levels[1].objects[1].rectangle.w = 1400;
    levels[1].objects[1].rectangle.h = 300;
    levels[1].objects[1].texture = IMG_LoadTexture(renderer, "textures/hill1.png");
    levels[1].objects[2].rectangle.x = 300;
    levels[1].objects[2].rectangle.y = 110;
    levels[1].objects[2].rectangle.w = 600;
    levels[1].objects[2].rectangle.h = 600;
    levels[1].objects[2].texture = IMG_LoadTexture(renderer, "textures/tree1.png");
    levels[1].objects[3].rectangle.x = 1000;
    levels[1].objects[3].rectangle.y = 130;
    levels[1].objects[3].rectangle.w = 600;
    levels[1].objects[3].rectangle.h = 600;
    levels[1].objects[3].texture = IMG_LoadTexture(renderer, "textures/tree2.png");
    
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    quit = true;
                break;
            default:
                break;
            }
        }
        
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        compute_position(levels, w);

        draw(renderer, levels);
        SDL_Delay(10);
    }
    
    
    quit_sdl(window, renderer, levels);
    return EXIT_SUCCESS;
}
