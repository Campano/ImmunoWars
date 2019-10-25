/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : masque.cpp
    Dernière Modification : 16/05/2010

    Description :
        Contient toutes les fonctions qui ont rapport au masque.
        Un masque est un tableau centré sur un pion qui renseigne
  	sur les cases qui lui sont accessibles et le nombre de coups
  	qu'il lui faut pour y parvenir.
*/
#include <stdio.h>
#include <stdlib.h>

#include "masque.h"

void masque_prgm(S_masque masque[MASQUE_MAX][MASQUE_MAX], S_pion* pion_jeu, S_case plateau[TAILLE_PLAT][TAILLE_PLAT])
{
   int i,j;
   coord case_jeu = pion_jeu->pos;
   coord org_mask = origine_masque(case_jeu);
   int longueur = pion_jeu->vitesse;
   int joueur_actuel = pion_jeu->type_pion;

   for(j=0;j<MASQUE_MAX;j++)        // initialisation du tableau
    {
        for(i=0;i<MASQUE_MAX;i++)
		{
			if(org_mask.x+i<0 || org_mask.x+i>=TAILLE_PLAT || org_mask.y+j<0 || org_mask.y+j>=TAILLE_PLAT)
			{//En-dehors plateau
				masque[i][j].occupe = BLOK;
				masque[i][j].poids=-1;
			}
			else if(plateau[org_mask.x+i][org_mask.y+j].type==CONTROLE && joueur_actuel == VIRUS)
			{//Prise contrôle par virus
				masque[i][j].occupe = TARG;
				masque[i][j].poids=-1;
			}
			else if(plateau[org_mask.x+i][org_mask.y+j].type!=NORMALE)
			{//Case occupée
				masque[i][j].occupe = BLOK;
				masque[i][j].poids=-1;
			}
			else if(plateau[org_mask.x+i][org_mask.y+j].pion!=NULL && plateau[org_mask.x+i][org_mask.y+j].pion->type_pion==joueur_actuel && plateau[org_mask.x+i][org_mask.y+j].pion != plateau[case_jeu.x][case_jeu.y].pion)
			{//Case amie
				masque[i][j].occupe = BLOK;
				masque[i][j].poids=-1;
			}
			else if(plateau[org_mask.x+i][org_mask.y+j].pion!=NULL && plateau[org_mask.x+i][org_mask.y+j].pion->type_pion!=joueur_actuel)
			{
				switch(plateau[org_mask.x+i][org_mask.y+j].pion->type_pion)
				{
					case GR :
						masque[i][j].occupe = NEUTRE;
						break;
					default :
						masque[i][j].occupe = OPPOSANT;
						break;
				}
				masque[i][j].poids=-1;
			}
			else
			{//Case libre
				masque[i][j].occupe=0;
				masque[i][j].poids=-1;
			}
		}
    }

	coord pos;
		pos.x = VITESSE_MAX;
		pos.y = VITESSE_MAX;

   parcours(masque,pos,0,longueur);
}

void parcours(S_masque masque[MASQUE_MAX][MASQUE_MAX], coord pos, int cpt, int longueur)
{
	/*
		Fonction récursive qui place le nombre de coups minimaux qu'il faut pour parvenir
		à une case sur toutes les cases du masque.
	*/
	int i;
	coord next_pos;

    if(masque[pos.x][pos.y].occupe!=BLOK || cpt==0) //si la case n'est pas bloquee ou amie
	{
		masque[pos.x][pos.y].poids=cpt;
		cpt++;

		if(masque[pos.x][pos.y].occupe==0 && cpt<=longueur)
			for(i=0; i<4; i++)
			{
				next_pos = get_pos(pos, i);
				if(poids(masque, next_pos)>cpt || poids(masque, next_pos)==-1)
					parcours(masque, next_pos, cpt, longueur);
			}

	}
}

coord get_pos(coord pos, int pos_rel)
{
	/*
		Renvoie les coordonées de la case
			au-dessus
			en-dessous
			en haut
			en abas
		pour la fonction de parcours
	*/
	switch (pos_rel) {
		case 0:
			pos.x--;
			break;
		case 1:
			pos.y--;
			break;
		case 2:
			pos.x++;
			break;
		default:
			pos.y++;
			break;
	}

	return pos;
}

int poids(S_masque masque[MASQUE_MAX][MASQUE_MAX], coord pos)
{
	/*
		Retourne le poids d'une case d'un masque si cette case fait
		bien partie du masque
	*/
	if(pos.x>=0 && pos.x<MASQUE_MAX && pos.y>=0 && pos.y<MASQUE_MAX)
		return masque[pos.x][pos.y].poids;
	else
		return 0;

}

coord origine_masque(coord centre_masque)
{
	//Retourne l'origine du masque
	   centre_masque.x -= VITESSE_MAX;
	   centre_masque.y -= VITESSE_MAX;

	   return centre_masque;
}

coord masque2tab(coord convert, coord centre_masque)
{
	//Convertit les coordonnées de masque en coordonnées de tableau
	coord org_masque = origine_masque(centre_masque);

	convert.x += org_masque.x;
	convert.y += org_masque.y;

	return convert;
}

coord tab2masque(coord convert, coord centre_masque)
{
	//Convertit lezs coordonnées de tableau en coordonnées de masque
	coord org_masque = origine_masque(centre_masque);

	convert.x -= org_masque.x;
	convert.y -= org_masque.y;

	return convert;
}

void chemin(S_masque masque[MASQUE_MAX][MASQUE_MAX], S_chemin** L_chemin, coord case_ini, coord fin)
{
	//Remplit une liste chaînée contenant les coordonnées successives du point de départ au point d'arrivée
	S_chemin* act = (S_chemin*) malloc(sizeof(S_chemin));
		act->etape = fin;
		act->suiv = NULL;

	fin = tab2masque(fin, case_ini);
	if(masque[fin.x][fin.y].poids>0)
	{
		if(masque[fin.x-1][fin.y].poids>=0 && masque[fin.x-1][fin.y].occupe==0 && masque[fin.x-1][fin.y].poids < masque[fin.x][fin.y].poids)
			fin.x--;
		else if(masque[fin.x+1][fin.y].poids>=0 && masque[fin.x+1][fin.y].occupe==0 && masque[fin.x+1][fin.y].poids < masque[fin.x][fin.y].poids)
			fin.x++;
		else if(masque[fin.x][fin.y-1].poids>=0 && masque[fin.x][fin.y-1].occupe==0 && masque[fin.x][fin.y-1].poids < masque[fin.x][fin.y].poids)
			fin.y--;
		else if(masque[fin.x][fin.y+1].poids>=0 && masque[fin.x][fin.y+1].occupe==0 && masque[fin.x][fin.y+1].poids < masque[fin.x][fin.y].poids)
			fin.y++;

		chemin(masque, L_chemin, case_ini, masque2tab(fin, case_ini));
	}

	enfiler_element_chemin(L_chemin, act);
}

/*FONCTIONS DE LISTE ADAPTEES AU TYPE S_CHEMIN, CF LISTES.CPP*/

void enfiler_element_chemin(S_chemin** L_deb, S_chemin* element)
{
	S_chemin* actuel;

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

void supprimer_chemin(S_chemin* element)
{
	if(element != NULL)
	{
		supprimer_chemin(element->suiv);
		free(element);
	}
}

