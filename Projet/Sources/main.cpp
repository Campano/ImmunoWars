/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : main.cpp
    Dernière Modification : 16/05/2010

    Description :
        Le main n'a pour but que de lancer partie() ou editeur_prgm(). Ainsi, il a ses propres surfaces qui ne sont utilisés que pour lui et lui seul.

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

#include "partie.h"
#include "editeur.h"
#include "outils_main.h"




int main(int argc, char *argv[])
{
/* CREATION DES SURFACES POUR LA SDL */
    SDL_Surface* tableau_surface_main[NB_SURFACE_MAIN];
    SDL_Rect tableau_position_main[NB_POS_MAIN];


/* DECLARATIONS DES VARIABLES */
    int menu=-1, son=1;
    FSOUND_STREAM *musique = NULL;

/* CHARGEMENT DES SURFACES */
    init_main(tableau_surface_main, tableau_position_main);

/* CHARGEMENT ET DEFINITION DE LA MUSIQUE */
    musique = FSOUND_Stream_Open("music/musique.mp3", FSOUND_LOOP_NORMAL, 0, 0);
    FSOUND_Stream_SetLoopCount(musique, -1);// Boucle à l'infini
    FSOUND_SetVolume(FSOUND_ALL, 255);// Reglage du volume


/* MENU PRINCIPAL */
    while(menu!=QUITTER)
    {
        FSOUND_Stream_Play(FSOUND_FREE, musique);// Lecture de la musique

        afficher_menu(tableau_surface_main, tableau_position_main, &son);

        choix_menu(tableau_surface_main, tableau_position_main, &menu, &son);

        if(menu==JOUER)
            {
                FSOUND_Stream_Stop(musique);// On arrete la musique
                partie();       //game.h
            }
        else if(menu==EDITEUR)
            {
                FSOUND_Stream_Stop(musique);
                editeur_prg(); // editeur.h
                SDL_ShowCursor(SDL_ENABLE); // Au cas où l'utilisateur a quitté l'éditeur en ayant le curseur désactivé.
            }

    }

    liberer_sdl_main(tableau_surface_main); // Libération des surfaces utiles au main.

    FSOUND_Stream_Close(musique);// Libération de l'espace mémoire
    FSOUND_Close(); // Quitte la bibliotheque de gestion de la musique

    return EXIT_SUCCESS;
}


