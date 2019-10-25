/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : tableau_jeu.h
    Dernière Modification : 16/05/2010

    Description :
        Header
*/

#ifndef TAB_JEU
	#define TAB_JEU

	#include "structures.h"
	#include "listes.h"

	int case_est_prenable(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], const coord case_ini, const coord case_jeu, const int joueur);
	void del_glob(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_pion* pion_temp, coord case_ini);
	void add_glob(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_pion* tab_listes[DIM_TAB_LISTES], S_pion* pion_temp, coord case_fin);

	void mitose(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_pion* tab_listes[DIM_TAB_LISTES], coord caract);

	int case_est_libre(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], coord case_jeu);
	int case_existe(coord case_jeu);
	int coord_eq(coord a, coord b);

	void attaque_virus(S_pion* virus, S_pion* globule);

#endif
