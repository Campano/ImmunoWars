#ifndef OUTILS_MAIN_H_INCLUDED
#define OUTILS_MAIN_H_INCLUDED

#define LARGEUR_FENETRE 620
#define HAUTEUR_FENETRE 730

#define NB_SURFACE_MAIN 8
#define NB_POS_MAIN 7

#define MENU_PRINCIPAL 0
#define TITRE_MENU 1
#define MENU1 2
#define MENU2 3
#define MENU3 4
#define IMAGE1 5
#define SON_ON 6
#define SON_OFF 7


#define QUITTER 0
#define JOUER 1
#define EDITEUR 2
#define SON 3

void init_main(SDL_Surface* tableau_surface_main[], SDL_Rect tableau_position_main[]);
void choix_menu(SDL_Surface* tableau_surface_main[], SDL_Rect tableau_position_main[], int* menu,int *son);
void afficher_menu(SDL_Surface* tableau_surface_main[], SDL_Rect tableau_position_main[], int* son);
void liberer_sdl_main(SDL_Surface* tableau_surface_main[]);


#endif // OUTILS_MAIN_H_INCLUDED
