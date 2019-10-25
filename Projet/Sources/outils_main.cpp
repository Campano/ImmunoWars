/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : outils_main.cpp
    Dernière Modification : 16/05/2010

    Description :
        Contient les fonctions utiles au main. Ce ne sont que des fonctions d'affichage,
        d'initialisation, de libération ou d'aquisition du choix du menu principal.
*/

#include <stdlib.h>
#include <stdio.h>

#include "structures.h"

#include <SDL/SDL.h>
#include <SDL/SDL_getenv.h>// Bibliotheque de positionement de fenetre

#ifdef WINDOWS
    #include <SDL/SDL_image.h> // Bibliotheque qui prend en charge tout les types d'images
    #include <SDL/SDL_ttf.h>
    #include <FMOD/fmod.h>
#endif

#ifdef MAC
    #include <SDL_image/SDL_image.h> // Bibliotheque qui prend en charge tout les types d'images
    #include <SDL_ttf/SDL_ttf.h>
    #include "fmod.h"
    #include "fmod_errors.h"
    #include "wincompat.h"
#endif

#include "outils_main.h"


void init_main(SDL_Surface* tableau_surface_main[], SDL_Rect tableau_position_main[])
/* PS : /
    Initialise et créé les surfaces et les positions.
*/
{
    int i;

    /* INITIALISATION DES SURFACES */
    for(i=0;i<=NB_SURFACE_MAIN;i++)
        tableau_surface_main[i]=NULL;

    /* INITIALISATION DES DIFFERENTS GESTIONNAIRES */
    SDL_Init(SDL_INIT_VIDEO); // chargement de la SDL
    TTF_Init();               // chargement du gestionnaire de police
    FSOUND_Init(44100, 32, 0);//chargement du gestionnaire de son

    /* OUVERTURE D'UNE FENETRE */
    putenv("SDL_VIDEO_WINDOW_POS=center"); //Centre la fenêtre par rapport à l'écran
    tableau_surface_main[MENU_PRINCIPAL] = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, SDL_HWSURFACE);
    SDL_WM_SetIcon(IMG_Load("pic/icones/icone_fenetre.png"), NULL); // Change l'icone de la fenètre
    SDL_WM_SetCaption("Immuno Wars", NULL); //Renome la fenetre



    /* CHARGEMENT DES IMAGES */
    tableau_surface_main[IMAGE1] = SDL_LoadBMP("pic/fonds/acceuil1.bmp");
        tableau_position_main[IMAGE1].x=0;
        tableau_position_main[IMAGE1].y=HAUTEUR_FENETRE-tableau_surface_main[IMAGE1]->h;
    tableau_surface_main[SON_ON] = IMG_Load("pic/icones/son_on.png");
    tableau_surface_main[SON_OFF] = IMG_Load("pic/icones/son_off.png");
        tableau_position_main[SON_ON].x=LARGEUR_FENETRE-tableau_surface_main[SON_ON]->w;
        tableau_position_main[SON_ON].y=HAUTEUR_FENETRE-tableau_surface_main[SON_ON]->h;


    /* CREATION DES POLICES */
    TTF_Font *police_titre = NULL, *police = NULL; // creation des polices
    police = TTF_OpenFont("polices/police_menu.ttf", 30);  // definition des polices
    police_titre = TTF_OpenFont("polices/police_menu_titre.ttf", 100);
    SDL_Color noir = {0,0,0};// creation d'une couleur


/* DEFINITION DES ZONES DE TEXTE */
    tableau_surface_main[TITRE_MENU] = TTF_RenderText_Blended(police_titre, "IMMUNO WARS",noir);
        tableau_position_main[TITRE_MENU].x=(LARGEUR_FENETRE-tableau_surface_main[TITRE_MENU]->w)/2; // centrage du texte
        tableau_position_main[TITRE_MENU].y=20;

    tableau_surface_main[MENU1] = TTF_RenderText_Blended(police, "JOUER", noir);
        tableau_position_main[MENU1].x=(LARGEUR_FENETRE-tableau_surface_main[MENU1]->w)/2; // centrage du texte
        tableau_position_main[MENU1].y=200;
    tableau_surface_main[MENU2] = TTF_RenderText_Blended(police, "EDITEUR", noir);
        tableau_position_main[MENU2].x=(LARGEUR_FENETRE-tableau_surface_main[MENU2]->w)/2; // centrage du texte
        tableau_position_main[MENU2].y=300;
    tableau_surface_main[MENU3] = TTF_RenderText_Blended(police, "QUITTER", noir);
        tableau_position_main[MENU3].x=(LARGEUR_FENETRE-tableau_surface_main[MENU3]->w)/2; // centrage du texte
        tableau_position_main[MENU3].y=400;

    TTF_CloseFont(police); // liberation des polices
    TTF_CloseFont(police_titre);
}

void choix_menu(SDL_Surface* tableau_surface_main[], SDL_Rect tableau_position_main[], int* menu,int *son)
/* PS: menu, son
    Attend le choix de l'utilisateur (clic de souris) pour redéfinir menu ou son.
*/
{
    *menu=-1;
    SDL_Event event;


    while(*menu==-1)
        {
            SDL_WaitEvent(&event);
            switch(event.type)
            {
                case SDL_QUIT: // Si l'utilisateur clic sur la croix de fermeture de fenetre
                    *menu=QUITTER;
                    break;
                case SDL_MOUSEBUTTONDOWN :
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if((event.button.x>=tableau_position_main[MENU1].x && event.button.x<=tableau_position_main[MENU1].x+tableau_surface_main[MENU1]->w) && (event.button.y>=tableau_position_main[MENU1].y && event.button.y<=tableau_position_main[MENU1].y+tableau_surface_main[MENU1]->h)) // Si le joueur clique sur 'jouer'
                            *menu=JOUER;
                        else if((event.button.x>=tableau_position_main[MENU2].x && event.button.x<=tableau_position_main[MENU2].x+tableau_surface_main[MENU2]->w) && (event.button.y>=tableau_position_main[MENU2].y && event.button.y<=tableau_position_main[MENU2].y+tableau_surface_main[MENU2]->h)) // Si le joueur clique sur 'editeur'
                            *menu=EDITEUR;
                        else if((event.button.x>=tableau_position_main[MENU3].x && event.button.x<=tableau_position_main[MENU3].x+tableau_surface_main[MENU3]->w) && (event.button.y>=tableau_position_main[MENU3].y && event.button.y<=tableau_position_main[MENU3].y+tableau_surface_main[MENU3]->h)) // Si le joueur clique sur 'quitter'
                            *menu=QUITTER;
                        else if((event.button.x>=tableau_position_main[SON_ON].x && event.button.x<=tableau_position_main[SON_ON].x+tableau_surface_main[SON_ON]->w) && (event.button.y>=tableau_position_main[SON_ON].y && event.button.y<=tableau_position_main[SON_ON].y+tableau_surface_main[SON_ON]->h))// Si le joueur clique sur l'icone du son
                            {
                                FSOUND_SetPaused(FSOUND_ALL, *son);
                                if(*son) // Si le son est actif
                                {
                                    *son=0; // Alors on l'arrete
                                    *menu=SON;
                                }
                                else
                                {
                                    *son=1; // Sinon on l'active
                                    *menu=SON;
                                }
                            }
                    }
                    break;
            }
        }
}

void afficher_menu(SDL_Surface* tab_s[], SDL_Rect tab_p[], int* son)
/* PS : /
    Affiche le menu principal
*/
{
        SDL_FillRect(tab_s[MENU_PRINCIPAL], NULL, SDL_MapRGB(tab_s[MENU_PRINCIPAL]->format, 255, 255, 255));

        SDL_BlitSurface(tab_s[IMAGE1], NULL, tab_s[MENU_PRINCIPAL], &tab_p[IMAGE1]);

        SDL_BlitSurface(tab_s[TITRE_MENU], NULL, tab_s[MENU_PRINCIPAL], &tab_p[TITRE_MENU]);

        SDL_BlitSurface(tab_s[MENU1], NULL, tab_s[MENU_PRINCIPAL], &tab_p[MENU1]);
        SDL_BlitSurface(tab_s[MENU2], NULL, tab_s[MENU_PRINCIPAL], &tab_p[MENU2]);
        SDL_BlitSurface(tab_s[MENU3], NULL, tab_s[MENU_PRINCIPAL], &tab_p[MENU3]);
        if(*son)
            SDL_BlitSurface(tab_s[SON_ON], NULL, tab_s[MENU_PRINCIPAL], &tab_p[SON_ON]);
        else
            SDL_BlitSurface(tab_s[SON_OFF], NULL, tab_s[MENU_PRINCIPAL], &tab_p[SON_ON]);

        SDL_Flip(tab_s[MENU_PRINCIPAL]);
}

void liberer_sdl_main(SDL_Surface* tableau_surface_main[])
/* PS : /
    Libère les surfaces en vue de quitter le programme
*/
{
    int i;
    for(i=0;i<NB_SURFACE_MAIN;i++)
       SDL_FreeSurface(tableau_surface_main[i]);

    TTF_Quit(); // Quitte la bibliotheque de gestion du texte
    SDL_Quit(); // Quitte la SDL

}

