/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : ia.h
    Dernière Modification : 16/05/2010

    Description :
        Header
*/

#ifndef FILE_IA
	#define FILE_IA

	#define COUPS_GB 3
	#define COUPS_VIRUS = (COUPS_GB)

	#define PROFONDEUR_MINMAX 3


	#include "structures.h"
	#include "masque.h"
	#include "listes.h"
	#include "tableau_jeu.h"

	struct S_deplacement
	{
		coord ini;
		coord fin;
	}; typedef struct S_deplacement S_deplacement;

	struct L_coups
	{
		coord ini;
		coord fin;
		struct L_coups* suiv;
	}; typedef struct L_coups L_coups;

	void creer_liste_filtree(S_pion** L_deb_a_jouer, S_case plateau_jeu[TAILLE_PLAT][TAILLE_PLAT], S_pion* tab_listes[DIM_TAB_LISTES], int joueur);
	int is_movable(S_case plat[TAILLE_PLAT][TAILLE_PLAT], S_pion* pion);
	void meilleur_coup(S_case plateau_jeu[TAILLE_PLAT][TAILLE_PLAT], S_pion* tab_listes[DIM_TAB_LISTES], coord tab_points_clefs[DIM_TAB_POINTS_CLEFS], coord* depart, coord* arrivee);
	void smart_move(S_pion* pion, S_case plateau_jeu[TAILLE_PLAT][TAILLE_PLAT], int* note_max, coord* coup_tmp, coord targ);

#endif
