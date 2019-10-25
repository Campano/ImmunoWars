/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : outils_sdl.cpp
    Dernière Modification : 16/05/2010

    Description :
        Contient les fonctions utiles de la sdl (conversions, initialisation et libération)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "structures.h"

#include <SDL/SDL.h>

#ifdef WINDOWS
    #include <SDL/SDL_image.h> // Bibliotheque qui prend en charge tout les types d'images
    #include <SDL/SDL_ttf.h>
#endif

#ifdef MAC
    #include <SDL_image/SDL_image.h> // Bibliotheque qui prend en charge tout les types d'images
    #include <SDL_ttf/SDL_ttf.h>
#endif

#include "outils_sdl.h"




int cons2sdl (int x)
/* PS : La valeur en pixel
    Converti la valeur d'une coordonnée du plateau de jeu en pixels
*/
{
    return 20*x+1;
}

int sdl2cons (int x, char choix)
/* PS : La valeur en coordonnée du plateau de jeu
    Converti une valeur en pixel. Il y a deux cas possible, si la fenetre est paramétrée differement. On laisse la fonctionnalité en cas d'évolutivité.
*/
{
    if(choix=='h')
        return (int) floor((x-POS_PLATEAU_X)/20);
    else
        {
            return (int)  floor((x-POS_PLATEAU_Y)/20);
        }
}

void initialiser_SDL(SDL_Surface* tableau_surface[],SDL_Rect tableau_position[])
/* PS : /
    Initialise et définie les surfaces et positions dont la sdl a besoin.
*/
{
    int i;

/* INITIALISATION DES SURFACES */
    for(i=0;i<NB_SURFACE;i++)
        tableau_surface[i]=NULL;


/* DEFINITION DES SURFACES */
    tableau_surface[ECRAN] = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, RESOLUTION, SDL_HWSURFACE);// creation fenetre
    SDL_FillRect(tableau_surface[ECRAN], NULL, SDL_MapRGB(tableau_surface[ECRAN]->format, 100, 100, 100));

    tableau_surface[EN_TETE] = SDL_CreateRGBSurface(SDL_HWSURFACE, 620, 120, RESOLUTION, 0, 0, 0, 0);
        SDL_FillRect(tableau_surface[EN_TETE], NULL, SDL_MapRGB(tableau_surface[ECRAN]->format, 70, 33, 220));
        tableau_position[EN_TETE].x=0;
        tableau_position[EN_TETE].y=0;

    tableau_surface[PLATEAU] = SDL_CreateRGBSurface(SDL_HWSURFACE, 602, 602, RESOLUTION, 0, 0, 0, 0);
        SDL_FillRect(tableau_surface[PLATEAU], NULL, SDL_MapRGB(tableau_surface[ECRAN]->format, 255, 255, 255));
        tableau_position[PLATEAU].x=POS_PLATEAU_X;
        tableau_position[PLATEAU].y=POS_PLATEAU_Y;

    tableau_surface[QUADRILLAGE_H] = SDL_CreateRGBSurface(SDL_HWSURFACE, 602, 2, RESOLUTION, 0, 0, 0, 0);
        SDL_FillRect(tableau_surface[QUADRILLAGE_H], NULL, SDL_MapRGB(tableau_surface[ECRAN]->format, 0, 0, 0));
        SDL_SetAlpha(tableau_surface[QUADRILLAGE_H], SDL_SRCALPHA, 200);

    tableau_surface[QUADRILLAGE_V] = SDL_CreateRGBSurface(SDL_HWSURFACE, 2, 602, RESOLUTION, 0, 0, 0, 0);
        SDL_FillRect(tableau_surface[QUADRILLAGE_V], NULL, SDL_MapRGB(tableau_surface[ECRAN]->format, 0, 0, 0));
        SDL_SetAlpha(tableau_surface[QUADRILLAGE_V], SDL_SRCALPHA, 200);


    tableau_surface[FOND_ECRAN] = SDL_LoadBMP("pic/fonds/fond_ecran_jeu.bmp");

    /* COLLAGE DU QUADRILLAGE DU PLATEAU DE JEU */
    tableau_position[0].y=0;
    for(i=0;i<=TAILLE_PLAT+4;i++)
    {
        tableau_position[0].x=i*20;
        SDL_BlitSurface(tableau_surface[QUADRILLAGE_V], NULL, tableau_surface[FOND_ECRAN], &tableau_position[0]);
    }

    tableau_position[0].x=0;
    for(i=0;i<=TAILLE_PLAT+4;i++)
    {
        tableau_position[0].y=i*20;
        SDL_BlitSurface(tableau_surface[QUADRILLAGE_H], NULL, tableau_surface[FOND_ECRAN], &tableau_position[0]);
    }

    /* CHARGEMENT DES IMAGES */
    tableau_surface[GB1_1] = IMG_Load("pic/pions/gb1_1.png");
    tableau_surface[GB1_2] = IMG_Load("pic/pions/gb1_2.png");
    tableau_surface[GB1_3] = IMG_Load("pic/pions/gb1_3.png");

    tableau_surface[GB2_1] = IMG_Load("pic/pions/gb2_1.png");
    tableau_surface[GB2_2] = IMG_Load("pic/pions/gb2_2.png");
    tableau_surface[GB2_3] = IMG_Load("pic/pions/gb2_3.png");

    tableau_surface[GB3_1] = IMG_Load("pic/pions/gb3_1.png");
    tableau_surface[GB3_2] = IMG_Load("pic/pions/gb3_2.png");
    tableau_surface[GB3_3] = IMG_Load("pic/pions/gb3_3.png");

    tableau_surface[VIR1] = IMG_Load("pic/pions/virus1.png");
    tableau_surface[VIR2] = IMG_Load("pic/pions/virus2.png");
    tableau_surface[VIR3] = IMG_Load("pic/pions/virus3.png");
    tableau_surface[E_VIRUS] = SDL_LoadBMP("pic/pt_clefs/entree_virus.bmp");

    tableau_surface[GROUGE1] = IMG_Load("pic/pions/gr1.png");
    tableau_surface[GROUGE2] = IMG_Load("pic/pions/gr2.png");

    tableau_surface[E_GR] = SDL_LoadBMP("pic/pt_clefs/entree_gr.bmp");

    tableau_surface[CTRL] = SDL_LoadBMP("pic/pt_clefs/pt_controle.bmp");

    tableau_surface[BLOQ] = SDL_LoadBMP("pic/pt_clefs/bloquee.bmp");

    tableau_surface[JOUEUR_GB] = SDL_LoadBMP("pic/en_tete/joueur_gb.bmp");
    tableau_surface[JOUEUR_VIR] = SDL_LoadBMP("pic/en_tete/joueur_vir.bmp");

    tableau_surface[MASQUE] = SDL_LoadBMP("pic/pt_clefs/masque.bmp");
    SDL_SetAlpha(tableau_surface[MASQUE], SDL_SRCALPHA, 170);

    tableau_surface[BUILDER] = SDL_LoadBMP("pic/en_tete/en_tete_builder.bmp");

    tableau_surface[TOTO1] = IMG_Load("pic/toto/toto1.png");
    tableau_surface[TOTO2] = IMG_Load("pic/toto/toto2.png");
    tableau_surface[TOTO3] = IMG_Load("pic/toto/toto3.png");
    tableau_surface[TOTO4] = IMG_Load("pic/toto/toto4.png");
    tableau_position[TOTO].x = 10;
    tableau_position[TOTO].y = 10;

}

void liberer_SDL(SDL_Surface* tableau_surface[])
/* PS : /
    Libère l'éspace mémoire dont la sdl a eu besoin
*/
{
    int i;

    for(i=0;i<NB_SURFACE;i++)
    {
        SDL_FreeSurface(tableau_surface[i]);
    }

}
