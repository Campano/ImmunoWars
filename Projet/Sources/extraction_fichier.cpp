/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : extraction_fichier.cpp
    Dernière Modification : 16/05/2010

    Description :
        Contient la fonction generer_plateau, qui extrait
    les informations formatées contenues dans map.txt.
    Ces informations donnent les positions des différents
    éléments d'un plateau de jeu donné.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "extraction_fichier.h"

void generer_plateau(S_case plateau_jeu[TAILLE_PLAT][TAILLE_PLAT], S_pion* tab_listes[DIM_TAB_LISTES], coord tab_points_clefs[DIM_TAB_POINTS_CLEFS])
{
    /* DECLARATION DES VARIABLES */

    //tableau qui va stocker les adresses des pions
	coord element_a_placer;

    //La variable de fichier
	FILE* map = NULL;

    char c;     //utile au parcours du fichier
    int i, j, extract, pion, type;

	//On remplit le plateau avec les valeurs par défaut
	for(i=0; i<TAILLE_PLAT; i++)
	{
		for(j=0; j<TAILLE_PLAT; j++)
		{
			plateau_jeu[i][j].pion = NULL;
			plateau_jeu[i][j].type = NORMALE;
		}
	}


    /* OUVERTURE DU FICHIER */

    map = fopen("map/map.txt", "r");



    if(map != NULL)
    {
        for(extract=0; extract<NB_TYPES_A_EXTRAIRE; extract++)
        {
            //On se place après le % (séparateur) suivant
            do c = fgetc(map); while(c != '#' && c != EOF);
            fgetc(map);

            //En fonction de la ligne dans le fichier, on trouve le type d'élément à placer
            ligne2pion(extract, &pion, &type);

            i=0;
            do
            {
                c = fgetc(map);

                if(c == '(')
                {
					//On recupere les coordonnees de l'element a placer
					fscanf(map, "%d;%d)", &(element_a_placer.x), &(element_a_placer.y));

					//On les ré-indexe correctement
					element_a_placer.x--;
					element_a_placer.y--;

					//On remplit le tableau et les listes
					if(pion != CASE)
					{
						//Création du pion dans la liste et allocation.
						plateau_jeu[element_a_placer.x][element_a_placer.y].pion = nouveau_pion(pion, tab_listes);
						plateau_jeu[element_a_placer.x][element_a_placer.y].pion->pos = element_a_placer;
					}
					if(type != NORMALE)
					{
						plateau_jeu[element_a_placer.x][element_a_placer.y].type = type;
						tab_points_clefs[type] = element_a_placer;
					}

					i++;
                }
            }while(c != '\n' && c != EOF);
        }

        fclose(map);
    }

}

void ligne2pion(int ligne, int* pion, int* type)
{
    //En fonction de la ligne, on choisit quel item il faut placer sur le plateau et dans les listes.
    switch(ligne)
    {
        case 0:
            *pion = GLOBULE;
            *type = NORMALE;
            break;
        case 1:
            *pion = VIRUS;
            *type = NORMALE;
            break;
        case 2:
            *pion = CASE;
            *type = BLOQUEE;
            break;
        case 3:
            *pion = CASE;
            *type = CONTROLE;
            break;
        case 4:
            *pion = CASE;
            *type = ENTREE_VIRUS;
            break;
        case 5:
            *pion = CASE;
            *type = ENTREE_GR;
            break;
        default:
            *pion = CASE;
            *type = NORMALE;
            break;
    }
}
