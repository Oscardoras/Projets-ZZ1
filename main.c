#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#define whWindow 100 //Longueur ET largeur fenêtres
#define pi 3.1415
#define nb_tours 2
#define nb_fenetres 15



int min(int a, int b) {return (a<b)?a:b;}



int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  if (SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Erreur init - %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }



  int i;
  int tetta; //décalage en degré (donc chgm en rad) pour les cos/sin

  SDL_Window ** windows = malloc(sizeof(SDL_Window *) * nb_fenetres);
  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);
  int w_display = DM.w;
  int h_display = DM.h;
  int dist = min(w_display/2, h_display/2); //Minimum distance centre-bords écran
  int x_centre = (w_display - whWindow)/2;
  int y_centre = (h_display - whWindow)/2;
  

  //-------------------------
  //- Création des fenêtres -
  //-------------------------

  for(i=0; i<nb_fenetres; i++) {
    
    windows[i] = SDL_CreateWindow("Fenêtre",
      x_centre + dist*cos(2*pi*i/nb_fenetres),
      y_centre + dist*sin(2*pi*i/nb_fenetres),
      100, 100, SDL_WINDOW_RESIZABLE
    );

    if (windows[i] == NULL) {
      SDL_Log("Erreur init fenêtre - %s\n", SDL_GetError());
      SDL_Quit();
      exit(EXIT_FAILURE);
    }

    SDL_Delay(125);
  }

  //----------------------------
  //- Déplacement des fenêtres -
  //----------------------------

  for(tetta=0; tetta<360*nb_tours; tetta++) {
    for(i=0; i<nb_fenetres; i++) {
      SDL_SetWindowPosition(windows[i],
        x_centre + dist*cos(2*pi*i/nb_fenetres + tetta*pi/180),
        y_centre + dist*sin(2*pi*i/nb_fenetres + tetta*pi/180)
      );
    }
    SDL_Delay(25);
  }


  SDL_Delay(3000);

  for(i=0; i<nb_fenetres; i++) {
    SDL_DestroyWindow(windows[i]);
  }

  SDL_Quit();

  return 0;
}