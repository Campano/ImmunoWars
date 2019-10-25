/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : editeur.h
    Dernière Modification : 16/05/2010

    Description :
        Header
*/

#ifndef EDITEUR_H_INCLUDED
#define EDITEUR_H_INCLUDED

#include "structures.h"
#include "affichage_sdl.h"
#include "outils_sdl.h"

void editeur_prg();
void initialiser_plateau(S_case plateau[TAILLE_PLAT][TAILLE_PLAT]);
void new_coord(S_chemin** L_deb, int x, int y);
void suppr_elt(S_chemin* element);
void fprintCoord(FILE* fichier, S_chemin* L_coord);
void sauvegarde(S_case plateau[TAILLE_PLAT][TAILLE_PLAT]);

#endif // EDITEUR_H_INCLUDED
