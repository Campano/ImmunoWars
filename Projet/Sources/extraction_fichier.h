/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : extraction_fichier.h
    Dernière Modification : 16/05/2010

    Description :
        Header
*/
#ifndef EXTRACTION_FICHIER
	#define EXTRACTION_FICHIER

	#include "structures.h"
	#include "listes.h"

	#define ADDR_LEN 300
	#define NB_TYPES_A_EXTRAIRE 6

	void generer_plateau(S_case plateau_jeu[TAILLE_PLAT][TAILLE_PLAT], S_pion* tab_listes[DIM_TAB_LISTES], coord tab_points_clefs[DIM_TAB_POINTS_CLEFS]);
	void ligne2pion(int ligne, int* pion, int* type);

#endif
