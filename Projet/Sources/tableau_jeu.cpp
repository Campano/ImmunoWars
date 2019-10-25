/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : jeu.cpp
    Dernière Modification : 16/05/2010

    Description :
        Contient les fonctions appellées depuis
	jeu.cpp, les noms des fonctions sont explicites
	dans leur grande majorité
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tableau_jeu.h"

int case_est_prenable(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], const coord case_ini, const coord case_jeu, const int joueur)
{
	S_pion* pion_ini = plateau[case_ini.x][case_ini.y].pion;
	S_pion* pion_fin = plateau[case_jeu.x][case_jeu.y].pion;
	int retour = 1;

	/*
		Une case n'est prenable que si celle si
			-n'est pas une case bloquée (elle est NORMALE)
			-ne contient pas un pion de la meme famille

	*/

	if(plateau[case_jeu.x][case_jeu.y].type != NORMALE)
		retour = 0;
    else if(pion_fin!=NULL)
        if(pion_fin!=pion_ini && pion_fin->type_pion==joueur)
            retour = 0;

	return retour;
}

void del_glob(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_pion* pion_temp, coord case_ini)
{
	//Supprime un grand pion
	int x,y;
	for(x=0; x<=1 ; x++)
		for(y=0; y<=1; y++)
			if(plateau[case_ini.x + x][case_ini.y + y].pion == pion_temp)
				plateau[case_ini.x + x][case_ini.y + y].pion = NULL;
}

void add_glob(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_pion* tab_listes[DIM_TAB_LISTES], S_pion* pion_temp, coord case_fin)
{
	//Place un grand pion en fonction de son niveau (GLOBULE)
	int x_p, y_p, x_max, y_max;
	int diviser = 0;
	x_max = pion_temp->niveau > 1 ? 1 : 0;
	y_max = pion_temp->niveau > 2 ? 1 : 0;

	for(y_p=0; y_p <= y_max; y_p++)
		for(x_p=0; x_p <= x_max; x_p++){
			if(plateau[ case_fin.x + x_p ][ case_fin.y + y_p ].pion!=NULL)
			{
				if(plateau[ case_fin.x + x_p ][ case_fin.y + y_p ].pion->type_pion == GR)
				{
					if(pion_temp->niveau == 3)
						diviser = 1;
					supprimer_element(&tab_listes[GR], plateau[ case_fin.x + x_p ][ case_fin.y + y_p ].pion);
				}
				//Supprimer de la liste des virus
				if(plateau[ case_fin.x + x_p ][ case_fin.y + y_p ].pion->type_pion == VIRUS)
					supprimer_element(&tab_listes[VIRUS], plateau[ case_fin.x + x_p ][ case_fin.y + y_p ].pion);
			}
			plateau[ case_fin.x + x_p ][ case_fin.y + y_p ].pion = pion_temp;
		}

	plateau[case_fin.x][case_fin.y].pion->pos = case_fin;

	if( diviser == 1 )
		mitose(plateau, tab_listes, case_fin);
}

void mitose(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_pion* tab_listes[DIM_TAB_LISTES], coord caract)
{
	plateau[caract.x][caract.y].pion->niveau = 1;

	plateau[caract.x+1][caract.y+1].pion = nouveau_pion(GLOBULE, tab_listes);
	plateau[caract.x+1][caract.y+1].pion->pos.x = caract.x+1;
	plateau[caract.x+1][caract.y+1].pion->pos.y = caract.y+1;

	plateau[caract.x][caract.y+1].pion = NULL;
	plateau[caract.x+1][caract.y].pion = NULL;
}

int case_est_libre(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], coord case_jeu)
{
	if(case_existe(case_jeu)==1 && plateau[case_jeu.x][case_jeu.y].type==NORMALE && plateau[case_jeu.x][case_jeu.y].pion == NULL)
		return 1;
	else
		return 0;
}

int case_existe(coord case_jeu)
{
	if(case_jeu.x>=0 && case_jeu.x<TAILLE_PLAT && case_jeu.y>=0 && case_jeu.y<TAILLE_PLAT)
		return 1;
	else
		return 0;
}

int coord_eq(coord a, coord b)
{
	if(a.x==b.x && a.y==b.y)
		return 1;
	else
		return 0;
}

void attaque_virus(S_pion* virus, S_pion* globule)
{
	switch(virus->niveau)
	{
		case 1 :
			globule->vitesse = (int) ceil(0.25*globule->vitesse); //math.h
			break;
		case 2 :
			globule->vitesse = 0;
			break;
		default :
			globule->niveau--;
			break;
	}
}
