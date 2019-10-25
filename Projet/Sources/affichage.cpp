#include <stdio.h>
#include <stdlib.h>

#include "affichage.h"

void demander_coord(coord* case_a_jouer, int choix)
{
	switch(choix)
	{
		case 1:
			NL() printf("Selectionnez le pion a deplacer"); NL()
			break;
		default:
			NL() printf("Selectionnez la destination"); NL()
			break;
	}

	do
	{
		printf("\tVeuillez saisir l'abscisse de la case : ");
		scanf("%d", &(case_a_jouer->x));
	}while(case_a_jouer->x < 1 || case_a_jouer->x > TAILLE_PLAT);

	do
	{
		printf("\tVeuillez saisir l'ordonnée de la case : ");
		scanf("%d", &(case_a_jouer->y));
	}while(case_a_jouer->y < 1 || case_a_jouer->y > TAILLE_PLAT);

	case_a_jouer->x--;
	case_a_jouer->y--;
}

void afficher_joueur(const int joueur)
{
	CLS()

	switch(joueur)
	{
		case GLOBULE:
			printf("C'est aux Globules Blancs de jouer");
			break;
		case VIRUS:
			printf("C'est aux Virus de jouer");
			break;
		default:
			printf("Erreur: le joueur actuel n'est ni les virus, ni les globules");
			break;
	}

	NLL()
}

char int2char(int nb)
{
	if( nb/10 < 1 )
		return 48+nb;
	else
		return 0;
}

void afficher_plateau(S_case plateau[TAILLE_PLAT][TAILLE_PLAT])
{
	int i,j;
	char affichage;
	for(j=-1; j<TAILLE_PLAT; j++)
	{
		if(j<0) printf("  | ");
		else printf("%d | ", j+1);


	   for(i=0; i<TAILLE_PLAT; i++)
	   {
			if(j==-1)
			{
				affichage = int2char(i+1);
			}
			else switch(plateau[i][j].type)
			{
				case NORMALE:
					if(plateau[i][j].pion==NULL)
						affichage = CHAR_VIDE;
					else if((plateau[i][j].pion)->type_pion == GLOBULE)
						affichage = CHAR_GLOB;
					else if((plateau[i][j].pion)->type_pion == VIRUS)
						affichage = CHAR_VIR;
					else
						affichage = CHAR_GR;
					break;
				case BLOQUEE:
					affichage = CHAR_BLOQ;
					break;
				case CONTROLE:
					affichage = CHAR_CTRL;
					break;
				case ENTREE_VIRUS:
					affichage = CHAR_EVIR;
					break;
				case ENTREE_GR:
					affichage = CHAR_EGR;
					break;
			}

			printf("%c ", affichage);

			if(i<TAILLE_PLAT-1) printf("| ");
	   }
		printf("\n");
	}
}
