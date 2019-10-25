/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : listes.h
    Dernière Modification : 16/05/2010

    Description :
        Header
*/
#ifndef LISTES
	#define LISTES

	#include "structures.h"

	S_pion* nouveau_pion(int type_pion, S_pion* tab_listes[DIM_TAB_LISTES]);
	void enfiler_element(S_pion** L_deb, S_pion* element);
	void supprimer_element(S_pion** L_deb, S_pion*element);
	void dupliquer_liste(S_pion** L_deb_2, S_pion* L_deb);
	void supprimer_liste(S_pion* element);

#endif
