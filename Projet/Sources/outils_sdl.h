/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : outils_sdl.h
    Dernière Modification : 16/05/2010

    Description :
        Header
*/

#ifndef OUTILS_SDL_H_INCLUDED
#define OUTILS_SDL_H_INCLUDED

#include "structures.h"
#include "affichage_sdl.h"

int cons2sdl (int x);
int sdl2cons (int x, char choix);
void initialiser_SDL(SDL_Surface* tableau_surface[],SDL_Rect tableau_position[]);
void liberer_SDL(SDL_Surface* tableau_surface[]);

#endif // OUTILS_SDL_H_INCLUDED
