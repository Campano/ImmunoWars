/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : ia.cpp
    Dernière Modification : 16/05/2010

    Description :
        Contient les fonctions et procédures nécéssaires au bon fonctionement de l'IA
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ia.h"

void meilleur_coup(S_case plateau_jeu[TAILLE_PLAT][TAILLE_PLAT], S_pion* tab_listes[DIM_TAB_LISTES], coord tab_points_clefs[DIM_TAB_POINTS_CLEFS], coord* depart, coord* arrivee)
{
	int eval = 0, eval_tmp = 0;
	S_pion* liste_pions = NULL;
	S_pion* pion;
	S_pion* pion_retenu;
	coord destination_tmp;

	//deplacement_pions(coup);
	creer_liste_filtree(&liste_pions, plateau_jeu, tab_listes, VIRUS);

	depart->x = -1;
	depart->y = -1;
	arrivee->x = -1;
	arrivee->y = -1;

	fprintf(stderr, "DEP : (%d;%d)", depart->x, depart->y);

	pion = liste_pions;
	while(pion!=NULL)
	{
		smart_move(pion, plateau_jeu, &eval_tmp, &destination_tmp, tab_points_clefs[CONTROLE]);

		if(eval_tmp > eval){
			eval = eval_tmp;
			pion_retenu = pion;
			*depart = pion_retenu->pos;
			*arrivee = destination_tmp;
		}
		pion = pion->suiv;
	}
		fprintf(stderr, "DEP2 : (%d;%d)", depart->x, depart->y);

	supprimer_liste(liste_pions);

}

void creer_liste_filtree(S_pion** L_deb_a_jouer, S_case plateau_jeu[TAILLE_PLAT][TAILLE_PLAT], S_pion* tab_listes[DIM_TAB_LISTES], int joueur)
{
	S_masque grand_masque[TAILLE_PLAT][TAILLE_PLAT];
	S_masque petit_masque[MASQUE_MAX][MASQUE_MAX];
	S_pion* act;
	S_pion* next;
	coord org_petit_masque;
	int i, j, is_attack = 0, niveau_max=0;

	//RE
	int opposant = joueur == GLOBULE ? VIRUS : GLOBULE;
	S_pion* pion = tab_listes[opposant];


	/******************************CREATION D'UN MASQUE PONDERE GRANDEUR NATURE************************/

	//Initialisation
	for(i=0; i<TAILLE_PLAT; i++)
		for(j=0; j<TAILLE_PLAT; j++)
		{
			grand_masque[i][j].occupe=0;
			grand_masque[i][j].poids=-1;
		}

	//Remplissage
	while(pion != NULL)
	{
		masque_prgm(petit_masque, pion, plateau_jeu);
		org_petit_masque = origine_masque(pion->pos);

		for(i=0; i<MASQUE_MAX; i++)
			for(j=0; j<MASQUE_MAX; j++)
			{
				if(org_petit_masque.x+i>=0 && org_petit_masque.x+i<TAILLE_PLAT && org_petit_masque.y+j >=0 && org_petit_masque.y+j< TAILLE_PLAT)
				{
					grand_masque[org_petit_masque.x+i][org_petit_masque.y+j].occupe = petit_masque[i][j].occupe;
					if(petit_masque[i][j].occupe == OPPOSANT && petit_masque[i][j].poids != -1)
						is_attack = 1;

					if(grand_masque[org_petit_masque.x+i][org_petit_masque.y+j].poids < petit_masque[i][j].poids)
						grand_masque[org_petit_masque.x+i][org_petit_masque.y+j].poids = petit_masque[i][j].poids;
				}
			}

		pion = pion->suiv;
	}


	/******************************CREATION D'UNE LISTE DES VIRUS FILTREE PAR NIVEAU************************/

	/*
		Ici on filtre la liste :

		Parcourir la liste
            Si un pion n'est pas bougeable
                Supprimer de la liste
			Si un pion est dans le champ d'un opposant ET le pion n'est pas dans le champ d'un opposant
				Supprimer de la liste
			Sinon
				MàJ le + haut niveau

		Parcourir la liste
			Si ce n'est pas un pion parmi les meilleurs
				Supprimer de la liste
	*/

	dupliquer_liste(L_deb_a_jouer, tab_listes[joueur]);

	act = *L_deb_a_jouer;
	next = *L_deb_a_jouer;

	while(act!=NULL)
	{
        next = act->suiv;

		if(!is_movable(plateau_jeu, act) || (is_attack==1 && grand_masque[act->pos.x][act->pos.y].poids == -1))
			supprimer_element(L_deb_a_jouer, act);
		else
			niveau_max = act->niveau > niveau_max ? act->niveau : niveau_max;

		act = next;
	}

	act = *L_deb_a_jouer;
	while(act != NULL)
	{
        next = act->suiv;
		if(act->niveau < niveau_max)
			supprimer_element(L_deb_a_jouer, act);
		act = next;
	}

}

int is_movable(S_case plat[TAILLE_PLAT][TAILLE_PLAT], S_pion* pion)
{
    coord h, b, g, d;

    h.x = (pion->pos).x;
        h = pion->pos;  h.y--;
        b = pion->pos;  b.y++;
        g = pion->pos;  g.x--;
        d = pion->pos;  d.x++;

    if(case_est_libre(plat, h) || case_est_libre(plat, b) || case_est_libre(plat, g) || case_est_libre(plat, d))
        return 1;
    else
        return 0;
}

void smart_move(S_pion* pion, S_case plateau_jeu[TAILLE_PLAT][TAILLE_PLAT], int* note_max, coord* coup_tmp, coord targ)
{
	S_masque masque_pion[MASQUE_MAX][MASQUE_MAX];
	int i, j;
	float dist=-1, dist_tmp;
	coord act;

	*note_max = 1;

	masque_prgm(masque_pion, pion, plateau_jeu);

	for(j=0; j<MASQUE_MAX; j++)
		for(i=0; i<MASQUE_MAX; i++)
		{
			if(masque_pion[i][j].poids != -1 && masque_pion[i][j].occupe > *note_max)
			{
				coup_tmp->x = i; coup_tmp->y = j;
				*coup_tmp = masque2tab(*coup_tmp, pion->pos);
				*note_max = masque_pion[i][j].occupe;
			}
			else if(masque_pion[i][j].poids != -1 && *note_max ==1)
			{
				act.x = i; act.y = j;
				act = masque2tab(act, pion->pos);
				dist_tmp = sqrt(pow(act.x-targ.x, 2) + pow(act.y-targ.y, 2));
				if(dist < 0 || dist_tmp < dist)
				{
					*coup_tmp=act;
					dist = dist_tmp;
				}
			}
		}
}
