/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : masque.h
    Dernière Modification : 16/05/2010

    Description :
        Header.
*/
#ifndef MASQUE_H_INCLUDED
	#define MASQUE_H_INCLUDED
	
		#include "structures.h"
	
		//Types de cases occupees
		#define BLOK 1
		#define NEUTRE 2
		#define TARG 3
		#define OPPOSANT 4
	
		struct S_chemin
		{
		    coord etape;
	        struct S_chemin* suiv;
		};typedef struct S_chemin S_chemin;
	
	void masque_prgm(S_masque masque[MASQUE_MAX][MASQUE_MAX], S_pion* pion_jeu, S_case plateau[TAILLE_PLAT][TAILLE_PLAT]);
	void parcours(S_masque masque[MASQUE_MAX][MASQUE_MAX], coord pos, int cpt, int longueur);
	coord get_pos(coord pos, int pos_rel);
	int poids(S_masque masque[MASQUE_MAX][MASQUE_MAX], coord pos);
	coord origine_masque(coord centre_masque);
	coord masque2tab(coord convert, coord centre_masque);
	coord tab2masque(coord convert, coord centre_masque);
	void chemin(S_masque masque[MASQUE_MAX][MASQUE_MAX], S_chemin** L_chemin, coord case_ini, coord fin);
	void enfiler_element_chemin(S_chemin** L_deb, S_chemin* element);
	void supprimer_chemin(S_chemin* element);

#endif // MASQUE_H_INCLUDED
