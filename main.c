#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#define GRAVITY 5
#define LONGUEUR 30
#define LARGEUR 20
#define VX_INIT 100
#define PI 3.14  



typedef struct{
  int vx;
  int x;
  int vy;
  int y;
  double angle;
  double angle_base;
  int diag;
} rect_t;



int min(int a, int b) {return (a<b)?a:b;}



void newRectangle(rect_t * rectangle, int xnew, int ynew) {
  rectangle->vx *= -1;
  rectangle->vy = 0;
  rectangle->x = xnew;
  rectangle->y = ynew;
  rectangle->angle = 0;
}



void physiqueRectangle(rect_t * rectangle, int ymax) {
  if((rectangle->y + rectangle->diag)/2 > ymax) {
    rectangle->vy *= -0.5;
    rectangle->y = ymax-(LONGUEUR/2);
  }
  rectangle->vy += GRAVITY;
  rectangle->y += rectangle->vy;
  rectangle->x += rectangle->vx;
  rectangle->angle = atan((rectangle->vy)/(rectangle->vx));
}



void drawRectangle(SDL_Renderer *renderer, rect_t * rectangle) {
  SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
  double angle1 = rectangle->angle_base + rectangle->angle;
  double angle2 = (PI - rectangle->angle_base) + rectangle->angle;
  double angle3 = -(PI - rectangle->angle_base) + rectangle->angle;
  double angle4 = -rectangle->angle_base + rectangle->angle;

  int x1 = rectangle->x + cos(angle1)*rectangle->diag/2;
  int y1 = rectangle->y + sin(angle1)*rectangle->diag/2;

  int x2 = rectangle->x + cos(angle2)*rectangle->diag/2;
  int y2 = rectangle->y + sin(angle2)*rectangle->diag/2;

  int x3 = rectangle->x + cos(angle3)*rectangle->diag/2;
  int y3 = rectangle->y + sin(angle3)*rectangle->diag/2;

  int x4 = rectangle->x + cos(angle4)*rectangle->diag/2;
  int y4 = rectangle->y + sin(angle4)*rectangle->diag/2;

  printf("%i, %i\n%i, %i\n%i, %i\n%i, %i\n\n\n", x1, y1, x2, y2, x3, y3, x4, y4);

  SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
  SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
  SDL_RenderDrawLine(renderer, x3, y3, x4, y4);
  SDL_RenderDrawLine(renderer, x4, y4, x1, y1);
}



int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  if (SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Erreur init - %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }


  int xnew = 0, ynew = 0;
  SDL_bool prog_on = SDL_TRUE;
  SDL_bool premierClic = SDL_FALSE;
  SDL_Event event;
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  rect_t * rectangle = malloc(sizeof(rect_t));

  rectangle->x = 0;
  rectangle->y = 0;
  rectangle->vx = 1;
  rectangle->vy = 0;
  rectangle->angle_base = -atan(LARGEUR/LONGUEUR);
  rectangle->diag = (int) sqrt(LONGUEUR*LONGUEUR + LARGEUR*LARGEUR);
  

  //-----------------------------------
  //- Création des fenêtres/renderers -
  //-----------------------------------
    
  window = SDL_CreateWindow("Fenêtre",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, 800, 500, SDL_WINDOW_RESIZABLE
  );
  if (window == NULL) {
    SDL_Log("Erreur init fenêtre - %s\n", SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
  }



  renderer = SDL_CreateRenderer(window, -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );
  if (renderer == NULL) {
    SDL_Log("Erreur init renderer - %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }



  while(prog_on) {

    if(SDL_PollEvent(&event)) {

      switch(event.type) {
        case(SDL_QUIT) :
          prog_on = SDL_FALSE;
          break;

        case(SDL_MOUSEBUTTONDOWN) :
          if(SDL_GetMouseState(&xnew, &ynew) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            newRectangle(rectangle, xnew, ynew);
            premierClic = SDL_TRUE;
          }
      }
    }


    SDL_SetRenderDrawColor(renderer, 50, 50, 70, 255);
    SDL_RenderClear(renderer);

    if (premierClic) {
      physiqueRectangle(rectangle, 500);

      SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
      drawRectangle(renderer, rectangle);
    }

    SDL_RenderPresent(renderer);

    SDL_Delay(100);
  }


  SDL_Delay(50);

  free(rectangle);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}