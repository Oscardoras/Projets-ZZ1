#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void draw(SDL_Texture* texture, SDL_Window *window,
                         SDL_Renderer *renderer, int pos) {
  SDL_Rect 
          source = {0},                         
          window_dimensions = {0},              
          destination = {0};                    

  SDL_GetWindowSize(
      window, &window_dimensions.w,
      &window_dimensions.h);                    
  SDL_QueryTexture(texture, NULL, NULL,
                   &source.w, &source.h);       
    
  int zoom = 5;
  destination.w = source.w * zoom;
  destination.h = source.h * zoom;
  destination.y = 
      (window_dimensions.h - destination.h) / 2;
  destination.x = (pos)%(window_dimensions.w + destination.w) - destination.w;

  SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
  SDL_RenderClear(renderer);
  
  SDL_RenderCopy(renderer, texture,
                 &source,
                 &destination);           
  SDL_RenderPresent(renderer);               
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Erreur init - %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
    }
    
    SDL_Window* window = SDL_CreateWindow("Anim",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        740,
        240,
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
    
    SDL_Texture* tab_textures[4] = {texture1, texture2, texture3};
    
    char nom[10] = "f0.png";
    
    for(int i=0; i<3; i++) {
        nom[1] = '1' + i;
        tab_textures[i] = IMG_LoadTexture(renderer, nom);
        if(!tab_textures[i]) {
            SDL_Log("Problem texture - %s", SDL_GetError());
            for(int j=0; j<i; j++) SDL_DestroyTexture(tab_textures[j]);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            exit(EXIT_FAILURE);
        }
    }
    
    tab_textures[3] = tab_textures[1];
    
    
    int quit = 0;
    int frame = 0;
    int pos = 0;
    SDL_Event event;
    
    while(!quit) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                quit = 1;
        }
        
        draw(tab_textures[frame], window, renderer, pos);
        
        frame = (frame+1)%4;
        pos = (pos<0) ? 720+37*5 : pos-30;
        
        SDL_Delay(200);
    }
}