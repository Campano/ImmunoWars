/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : jeu.cpp
    Derni�re Modification : 16/05/2010

    Description :
        Contient les fonctions appell�es depuis
	partie.cpp qui g�rent le moteur du jeu, toute
	la logique et les actions r�alisables.
		Certaines d'entre elles n�cessitent des
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
		Cette fonction d�place un virus ou un globule en g�rant les attaques
		
		
		case_ini & case_fin ont �t� v�rifi�es sont valides pour
		le d�placement

		R�cup�rer le pion
		
		Si c'est un globule
			Supprimer le globule de la case de d�part
			Ajouter le globule sur la case d'arriv�e, en g�rant 
			les phagocytoses (mange un autre pion si pr�sent)
			
		Si c'est un Virus
			Si la case d'arriv�e est la point de contr�le
				Supprimer le virus
				Incr�menter le point de contr�le
			Si la case d'arriv�e contient un globule
				Attaquer le globule
				Mettre � jour le globule
				Supprimer le virus
			Sinon
				Si la case d'arriv�e contient un globule rouge
					Supprimer le globule rouge
				D�placer le virus
			
	*/
	
	S_pion* pion_ini = plateau[case_ini.x][case_ini.y].pion;
	S_pion* pion_fin = plateau[case_fin.x][case_fin.y].pion;

	if(pion_ini->type_pion == GLOBULE) //M�nage dans les trois cases autour
	{
		del_glob(plateau, pion_ini, case_ini);
		add_glob(plateau, tab_listes, pion_ini, case_fin);
	}
	else
	{
		if(coord_eq(case_fin, pt_ctrl))
		{
			supprimer_element(&tab_listes[VIRUS], pion_ini);
			cpt_controle(1); //Incr�mente le compteur de virus ayant atteint le point de contr�le
		}
		else if( pion_fin == NULL || pion_fin->type_pion != GLOBULE )
		{
		    if(pion_fin !=NULL && pion_fin->type_pion == GR)
                supprimer_element(&tab_listes[GR], pion_fin);

			//D�place le pion
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
		Cette fonction permet de faire �voluer un certain nombre de globules
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
            	//On v�rifie que le globule a la place pour grossir
                A = L_gb->pos; A.x++;
                if(case_est_libre(plateau, A)==1)
                {
                    L_gb->niveau++;					//Le globule passe au niveau 2
                    plateau[A.x][A.y].pion = glob;	//On m�j le tableau en cons�quence
                    i++;	//On a fait �voluer un globule : le compteur augmente
                }
                break;
            case 2:
            	//M�me logique que le niveau deux, mais il faut rajouter deux pointeurs dans le tableau
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
		Cette fonction cr�e un pion du type voulu al�atoirement autour de son point d'entr�e
	*/
	int i,j;
	int creer = 0;
	coord sortie;

	srand(time(NULL)); //seed
	
	// On v�rifie d'abord qu'une case est disponible autour du point d'entr�e
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
	
	//On cr�e le pion si cela est possible
	if(creer==1)
	{
		//Recherche de coordonn�es al�atoires
		do
		{
			sortie.x=rand()%3; sortie.x = sortie.x == 2 ? -1 : sortie.x;	//Apr�s cette ligne, sortie.x appartient � { -1 ; 0 ; 1 }
			sortie.y=rand()%3; sortie.y = sortie.y == 2 ? -1 : sortie.y; 	//Idem
			sortie.x += pt_entree.x;		//On ajoute cela aux coordonn�es du point d'entr�e, et on obtient alors l'une des cases
			sortie.y += pt_entree.y;		//autour dudit point d'entr�e
		}while(coord_eq(sortie, pt_entree)==1 || case_est_libre(plateau, sortie)!=1); //Reste � v�rifier que la case est disponible

		//Cr�ation du pion
		plateau[sortie.x][sortie.y].pion = nouveau_pion(type_pion, tab_listes);	
		plateau[sortie.x][sortie.y].pion->pos = sortie;

		//Niveau al�atoire pour les virus
		if(type_pion == VIRUS)
            plateau[sortie.x][sortie.y].pion->niveau = rand()%3 + 1;
	}

}

coord case_caract(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], coord case_jeu)
{
	/*
		Renvoie la case caract�ristique d'un pion. Pour les globules de niveau
		sup�rieur � 1, c'est la case du globule la plus proche de l'origine
	*/
	if(plateau[case_jeu.x][case_jeu.y].pion!=NULL && plateau[case_jeu.x][case_jeu.y].pion->type_pion==GLOBULE)
		case_jeu = plateau[case_jeu.x][case_jeu.y].pion->pos;

	return case_jeu;
}

int verif_coord(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], const coord case_jeu, const int joueur)
{
	/*
		V�rifie qu'une case contient bien un pion appartenant au joueur voulu
		Retourne 1 si la condition pr�c�dente est vraie, 0 sinon
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
		V�rifie que la case d'arriv�e est effectivement disponible pour le pion
		de la case de d�part, n�cessite le masque pond�r� de ce pion
		1 si vrai, 0 si faux
	*/
	int x_masque = case_jeu.x - (case_ini.x - VITESSE_MAX);
	int y_masque = case_jeu.y - (case_ini.y - VITESSE_MAX);

	int retour = 0;

	if(x_masque>=0 && x_masque <MASQUE_MAX && y_masque>=0 && y_masque < MASQUE_MAX)
	{
		//On v�rifie que la case est disponible pour le pion (info donn�e par le masque)
        retour = masque[x_masque][y_masque].poids>0 ? 1 : 0;
		
		//Si le joueur est un globule de niveau sup�rieur � 1, il faut v�rifier les cases concern�es
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
	//Met � jour un globule blanc lorsque celui-ci a �t� attaqu� par un virus
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
		Renseigne, incr�mente ou remet � z�ro le compteur 
		de virus ayant atteint le point de contr�le
	*/
	static int cpt = 0;
	cpt = add == 1 ? ++cpt : cpt; //Incr�mentation pr�fix�e de rigueur
	cpt = add == 2 ? 0 : cpt;
	return cpt;
}

void deplacer_gr(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], S_pion* Grouge)
{
	/*
		D�place al�atoirement les globules rouges autour de leur position
	*/
	int j=-1, i=-1, creer = 0;
	coord sortie, pt_entree;

	srand(time(NULL));

	while(Grouge != NULL)
	{
		pt_entree = Grouge->pos;
		
		//Le code est tout � fait similaire au code d'entr�e de pions

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
