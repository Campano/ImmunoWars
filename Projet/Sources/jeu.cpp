/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : jeu.cpp
    Dernière Modification : 16/05/2010

    Description :
        Contient les fonctions appellées depuis
	partie.cpp qui gèrent le moteur du jeu, toute
	la logique et les actions réalisables.
		Certaines d'entre elles nécessitent des
	fonctions secondaires qui se trouvent alors 
	dans tableau_jeu.cpp
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "jeu.h"

void changer_joueur(int* joueur)
{
	//Passe au joueur suivant
	switch(*joueur)
	{
		case GLOBULE: *joueur = VIRUS; break;
		default: *joueur = GLOBULE; break;
	}
}

void deplacement(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_pion* tab_listes[DIM_TAB_LISTES], const coord case_ini, const coord case_fin, const coord pt_ctrl)
{
	/*
		Cette fonction déplace un virus ou un globule en gérant les attaques
		
		
		case_ini & case_fin ont été vérifiées sont valides pour
		le déplacement

		Récupérer le pion
		
		Si c'est un globule
			Supprimer le globule de la case de départ
			Ajouter le globule sur la case d'arrivée, en gérant 
			les phagocytoses (mange un autre pion si présent)
			
		Si c'est un Virus
			Si la case d'arrivée est la point de contrôle
				Supprimer le virus
				Incrémenter le point de contrôle
			Si la case d'arrivée contient un globule
				Attaquer le globule
				Mettre à jour le globule
				Supprimer le virus
			Sinon
				Si la case d'arrivée contient un globule rouge
					Supprimer le globule rouge
				Déplacer le virus
			
	*/
	
	S_pion* pion_ini = plateau[case_ini.x][case_ini.y].pion;
	S_pion* pion_fin = plateau[case_fin.x][case_fin.y].pion;

	if(pion_ini->type_pion == GLOBULE) //Ménage dans les trois cases autour
	{
		del_glob(plateau, pion_ini, case_ini);
		add_glob(plateau, tab_listes, pion_ini, case_fin);
	}
	else
	{
		if(coord_eq(case_fin, pt_ctrl))
		{
			supprimer_element(&tab_listes[VIRUS], pion_ini);
			cpt_controle(1); //Incrémente le compteur de virus ayant atteint le point de contrôle
		}
		else if( pion_fin == NULL || pion_fin->type_pion != GLOBULE )
		{
		    if(pion_fin !=NULL && pion_fin->type_pion == GR)
                supprimer_element(&tab_listes[GR], pion_fin);

			//Déplace le pion
			plateau[case_fin.x][case_fin.y].pion = pion_ini;		//Dans le tableau
			plateau[case_fin.x][case_fin.y].pion->pos = case_fin;	//Dans la liste
		}
		else //-> atteindre GB
		{
			attaque_virus(pion_ini, pion_fin);
			maj_glob(plateau, &tab_listes[GLOBULE], case_fin);
			supprimer_element(&tab_listes[VIRUS], pion_ini);
		}

		plateau[case_ini.x][case_ini.y].pion = NULL;
	}
}

void evolution_globules(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_pion* L_gb)
{
	/*
		Cette fonction permet de faire évoluer un certain nombre de globules
	*/
	S_pion* glob = NULL;
	int i = 0;
	coord A, B;
	
	//Parcours de la liste des globules
	while(i<NB_EVOL && L_gb != NULL)
	{
        switch(L_gb->niveau)
        {
        	//Si c'est un globule de niveau 1
            case 1:
            	//On vérifie que le globule a la place pour grossir
                A = L_gb->pos; A.x++;
                if(case_est_libre(plateau, A)==1)
                {
                    L_gb->niveau++;					//Le globule passe au niveau 2
                    plateau[A.x][A.y].pion = glob;	//On màj le tableau en conséquence
                    i++;	//On a fait évoluer un globule : le compteur augmente
                }
                break;
            case 2:
            	//Même logique que le niveau deux, mais il faut rajouter deux pointeurs dans le tableau
                A = L_gb->pos; A.x++; A.y++;
                B = L_gb->pos; B.y++;
                if(case_est_libre(plateau, A)==1 && case_est_libre(plateau, B)==1)
                {
                    L_gb->niveau++;
                    plateau[A.x][A.y].pion = L_gb;
                    plateau[B.x][B.y].pion = L_gb;
                    i++;
                }
                break;
            default: break;
        }
        L_gb = L_gb->suiv;
	}
}

void entree_pions(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_pion* tab_listes[DIM_TAB_LISTES], const coord pt_entree, const int type_pion)
{
	/*
		Cette fonction crée un pion du type voulu aléatoirement autour de son point d'entrée
	*/
	int i,j;
	int creer = 0;
	coord sortie;

	srand(time(NULL)); //seed
	
	// On vérifie d'abord qu'une case est disponible autour du point d'entrée
	for(i=-1; i<2; i++)
	{
		for(j=-1; j<2; j++)
		{
			sortie.x = i + pt_entree.x;
			sortie.y = j + pt_entree.y;
			if(case_est_libre(plateau, sortie)==1)
				creer=1;
		}
	}
	
	//On crée le pion si cela est possible
	if(creer==1)
	{
		//Recherche de coordonnées aléatoires
		do
		{
			sortie.x=rand()%3; sortie.x = sortie.x == 2 ? -1 : sortie.x;	//Après cette ligne, sortie.x appartient à { -1 ; 0 ; 1 }
			sortie.y=rand()%3; sortie.y = sortie.y == 2 ? -1 : sortie.y; 	//Idem
			sortie.x += pt_entree.x;		//On ajoute cela aux coordonnées du point d'entrée, et on obtient alors l'une des cases
			sortie.y += pt_entree.y;		//autour dudit point d'entrée
		}while(coord_eq(sortie, pt_entree)==1 || case_est_libre(plateau, sortie)!=1); //Reste à vérifier que la case est disponible

		//Création du pion
		plateau[sortie.x][sortie.y].pion = nouveau_pion(type_pion, tab_listes);	
		plateau[sortie.x][sortie.y].pion->pos = sortie;

		//Niveau aléatoire pour les virus
		if(type_pion == VIRUS)
            plateau[sortie.x][sortie.y].pion->niveau = rand()%3 + 1;
	}

}

coord case_caract(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], coord case_jeu)
{
	/*
		Renvoie la case caractéristique d'un pion. Pour les globules de niveau
		supérieur à 1, c'est la case du globule la plus proche de l'origine
	*/
	if(plateau[case_jeu.x][case_jeu.y].pion!=NULL && plateau[case_jeu.x][case_jeu.y].pion->type_pion==GLOBULE)
		case_jeu = plateau[case_jeu.x][case_jeu.y].pion->pos;

	return case_jeu;
}

int verif_coord(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], const coord case_jeu, const int joueur)
{
	/*
		Vérifie qu'une case contient bien un pion appartenant au joueur voulu
		Retourne 1 si la condition précédente est vraie, 0 sinon
	*/
	S_pion* pion = plateau[case_jeu.x][case_jeu.y].pion;
	int retour = 0;

	if( pion != NULL)
	{
		if(pion->type_pion == joueur)
			retour = 1;
	}

	return retour;
}

int is_valid(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_masque masque[MASQUE_MAX][MASQUE_MAX], coord case_ini, coord case_jeu, const int joueur, int encombrement_pion, coord pt_ctrl)
{
	/*
		Vérifie que la case d'arrivée est effectivement disponible pour le pion
		de la case de départ, nécessite le masque pondéré de ce pion
		1 si vrai, 0 si faux
	*/
	int x_masque = case_jeu.x - (case_ini.x - VITESSE_MAX);
	int y_masque = case_jeu.y - (case_ini.y - VITESSE_MAX);

	int retour = 0;

	if(x_masque>=0 && x_masque <MASQUE_MAX && y_masque>=0 && y_masque < MASQUE_MAX)
	{
		//On vérifie que la case est disponible pour le pion (info donnée par le masque)
        retour = masque[x_masque][y_masque].poids>0 ? 1 : 0;
		
		//Si le joueur est un globule de niveau supérieur à 1, il faut vérifier les cases concernées
        if(joueur==GLOBULE && retour==1 && encombrement_pion > 1)
        {
            case_jeu.x++;
            retour = case_est_prenable(plateau, case_ini, case_jeu, joueur);

            if(retour==1 && encombrement_pion==3)
            {
                case_jeu.y++;
                retour = case_est_prenable(plateau, case_ini, case_jeu, joueur);
                case_jeu.x--;
                retour += case_est_prenable(plateau, case_ini, case_jeu, joueur);

                retour = retour==2 ? 1 : 0;
            }
        }
	}

	return retour;
}

void maj_glob(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_pion** L_deb_glob, coord case_jeu)
{
	//Met à jour un globule blanc lorsque celui-ci a été attaqué par un virus
	coord caract = case_caract(plateau, case_jeu);
	S_pion* glob = plateau[caract.x][caract.y].pion;

	if(glob->niveau == 0)
	{
		supprimer_element(L_deb_glob, glob);
		plateau[caract.x][caract.y].pion = NULL;
	}
	else if(glob->niveau == 1)
	{
		plateau[caract.x+1][caract.y].pion = NULL;
	}
	else if(glob->niveau == 2)
	{
		plateau[caract.x][caract.y+1].pion = NULL;
		plateau[caract.x+1][caract.y+1].pion = NULL;
	}
}

int cpt_controle(int add)
{
	/*
		Renseigne, incrémente ou remet à zéro le compteur 
		de virus ayant atteint le point de contrôle
	*/
	static int cpt = 0;
	cpt = add == 1 ? ++cpt : cpt; //Incrémentation préfixée de rigueur
	cpt = add == 2 ? 0 : cpt;
	return cpt;
}

void deplacer_gr(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_pion* Grouge)
{
	/*
		Déplace aléatoirement les globules rouges autour de leur position
	*/
	int j=-1, i=-1, creer = 0;
	coord sortie, pt_entree;

	srand(time(NULL));

	while(Grouge != NULL)
	{
		pt_entree = Grouge->pos;
		
		//Le code est tout à fait similaire au code d'entrée de pions

		while(creer != 1 && !(i==1 && j==1))
		{
			if(i==2){
				i=-1;
				j++;
			}

			sortie.x = i + pt_entree.x;
			sortie.y = j + pt_entree.y;
			if(case_est_libre(plateau, sortie)==1)
				creer=1;
			i++;
		}

		if(creer==1)
		{
			do
			{
				sortie.x=rand()%3; sortie.x = sortie.x == 2 ? -1 : sortie.x;
				sortie.y=rand()%3; sortie.y = sortie.y == 2 ? -1 : sortie.y;
				sortie.x += pt_entree.x;
				sortie.y += pt_entree.y;
			}while(coord_eq(sortie, pt_entree)==1 || case_est_libre(plateau, sortie)!=1);

			plateau[pt_entree.x][pt_entree.y].pion = NULL;
			plateau[sortie.x][sortie.y].pion = Grouge;
			Grouge->pos = sortie;
		}

		Grouge = Grouge->suiv;
	}
}
