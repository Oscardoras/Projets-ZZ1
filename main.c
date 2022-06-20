#include <SDL2/SDL.h>
#include <stdio.h>



int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  SDL_Window *window = NULL;

  if (SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Erreur init - %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  window = SDL_CreateWindow("Fenêtre", 0, 0, 400, 300, SDL_WINDOW_RESIZABLE);

  if (window == NULL) {
    SDL_Log("Erreur init fenêtre - %s\n", SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  SDL_Delay(15000);

  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
