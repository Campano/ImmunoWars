/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : jeu.h
    Dernière Modification : 16/05/2010

    Description :
        Header
*/
#ifndef JEU
	#define JEU

	#include "structures.h"
	#include "tableau_jeu.h"
	#include "listes.h"

	void changer_joueur(int* joueur);

	void deplacement(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_pion* tab_listes[DIM_TAB_LISTES], const coord case_ini, const coord case_fin, const coord pt_ctrl);

	void evolution_globules(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_pion* L_gb);
	void entree_pions(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_pion** L_deb, const coord pt_entree, const int type_pion);

	coord case_caract(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], coord case_jeu);

	int verif_coord(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], const coord case_jeu, const int joueur);
	int is_valid(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_masque masque[MASQUE_MAX][MASQUE_MAX], coord case_ini, coord case_jeu, const int joueur, int encombrement_pion, coord pt_ctrl);

	void maj_glob(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_pion** L_deb_glob, coord case_jeu);
	int cpt_controle(int add);
	void deplacer_gr(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_pion* Grouge);
#endif

