#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define ZOOM_F 5
#define ZOOM_A 22.5
#define ZOOM_M 22.5

SDL_Rect fourmiAnim[4];

int main() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Erreur SDL init - %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
    }
    
    SDL_Window* window = SDL_CreateWindow("Anim",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        720,
        720,
        SDL_WINDOW_RESIZABLE
    );
    
    
    if(!window) {
        SDL_Log("Error Window - %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    
    //Textures
    SDL_Texture* textureFourmi = IMG_LoadTexture(renderer, "FourmiParallax.png");
    SDL_Texture* textureArbre = IMG_LoadTexture(renderer, "ParallaxArbre.png");
    SDL_Texture* textureMontagne = IMG_LoadTexture(renderer, "ParallaxMont.png");
    

    SDL_Rect
        source_f = {0},
        source_a = {0},
        source_m = {0},                    
        window_dimensions = {0},              
        destination_f = {0},
        destination_a = {0},
        destination_m = {0},
        fourmiAnim[4];


    SDL_GetWindowSize(
      window, &window_dimensions.w,
      &window_dimensions.h);
    
    //Dimensions des spritesheets
    SDL_QueryTexture(textureFourmi, NULL, NULL,
                   &source_f.w, &source_f.h);   
    SDL_QueryTexture(textureArbre, NULL, NULL,
                   &source_a.w, &source_a.h);
    SDL_QueryTexture(textureMontagne, NULL, NULL,
                   &source_m.w, &source_m.h);
    

    //Emplacement de chaque sprite de fourmi
    for(int i=0; i<4; i++) {
        fourmiAnim[i].x = 0;
        fourmiAnim[i].y = i*10;
        fourmiAnim[i].w = 36;
        fourmiAnim[i].h = 10;
    }


    //Emplacement des images sur l'écran
    destination_f.w = source_f.w * ZOOM_F;
    destination_f.h = source_f.h / 4 * ZOOM_F;
    destination_f.x = (window_dimensions.w - destination_f.w) / 2;
    destination_f.y = (window_dimensions.h - destination_f.h) / 1.1;

    destination_a.w = source_a.h * ZOOM_A;
    destination_a.h = source_a.h * ZOOM_A;
    destination_a.x = 0;
    destination_a.y = 0;

    destination_m.w = source_m.h * ZOOM_M;
    destination_m.h = source_m.h * ZOOM_M / 1.25;
    destination_m.x = 0;
    destination_m.y = -100;

        
    int quit = 0;
    int frame = 0;
    int pos = 0;
    SDL_Event event;
    
    while(!quit) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                quit = 1;
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 191, 255, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, textureMontagne, &source_m, &destination_m);
        SDL_RenderCopy(renderer, textureArbre, &source_a, &destination_a);
        SDL_RenderCopy(renderer, textureFourmi, &fourmiAnim[frame], &destination_f);
        SDL_RenderPresent(renderer);
        SDL_Delay(200);

        frame = (frame+1) % 4;
    }

    return 0;
}