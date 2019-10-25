/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : listes.cpp
    Dernière Modification : 16/05/2010

    Description :
        Contient toutes les fonctions permettant
	de manipuler les listes de pions, qui vont de
	pair avec le tableau de jeu
*/
#include <stdio.h>
#include <stdlib.h>

#include "listes.h"

S_pion* nouveau_pion(int type_pion, S_pion* tab_listes[DIM_TAB_LISTES])
{
	/*
		Cette fonction crée un pion et l'ajoute à la liste qui lui
		correspond.
	*/
	S_pion* element = (S_pion*) malloc(sizeof(S_pion)); //Allocation

	/*---------CARACTERISTIQUES DU PION--------*/
	element->type_pion = type_pion;
	element->niveau = 1;
	element->suiv = NULL;

	switch(type_pion)
	{
		case GLOBULE:
			element->vitesse = VITESSE_INI_GB;
			break;
		case VIRUS :
			element->vitesse = VITESSE_INI_VIRUS;
			break;
		default :
			element->vitesse = VITESSE_INI_GR;
			break;
	}

	/*---------AJOUT DU PION À LA LISTE--------*/
	enfiler_element(&tab_listes[type_pion], element);

	return element;
}

void enfiler_element(S_pion** L_deb, S_pion* element)
{
	/*
		Chaîne un élément à la fin d'une liste
	*/
	S_pion* actuel;

	if(*L_deb == NULL)
		*L_deb = element;
	else
	{
		actuel = *L_deb;
		while(actuel->suiv != NULL)
			actuel = actuel->suiv;
		actuel->suiv = element;
	}
}

void supprimer_element(S_pion** L_deb, S_pion* element)
{
	/*
		Supprime un élément d'une liste
	*/
	S_pion* prec = NULL;
	S_pion* actuel = *L_deb;

	//On recherche l'élement dans la liste
	while( actuel != element )
	{
        prec = actuel;
        actuel = prec->suiv;
	}
	
	//On modifie le chaînage dans la liste
    if(prec==NULL)
        *L_deb = actuel->suiv;
    else
        prec->suiv = actuel->suiv;

	//On peut alors supprimer le pion et libérer la mémoire
    free(actuel);
}

void dupliquer_liste(S_pion** L_deb_2, S_pion* L_deb)
{
	/*
		Cette fonction crée une copie de la liste L_deb
		dans L_deb_2. Les deux sont strictement identiques,
		mais les actions sur l'une n'auront pas d'effet sur l'autre.
	*/
	S_pion* act1 = L_deb;
	S_pion* nouveau = NULL;

	//Parcours de la liste en entier
	while(act1 != NULL)
	{
		//on crée une copie de chaque élément act1
		nouveau = (S_pion*) malloc(sizeof(S_pion));
		*nouveau = *act1;
		nouveau->suiv = NULL;
		
		//On le chaîne
		enfiler_element(L_deb_2, nouveau);
		
		act1 = act1->suiv;
	}
}

void supprimer_liste(S_pion* element)
{
	/*
		Supprime tous les éléments d'une liste
		en passant le premier element en argument
		Attention, le pointeur de début de liste
		n'est pas mis à NULL.
	*/
	if(element != NULL)
	{
		supprimer_liste(element->suiv);
		free(element);
	}
}

