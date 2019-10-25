/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : partie.cpp
    Derni�re Modification : 16/05/2010

    Description :
        Il contient le programme de jeu. Noyau du programme, il est le 'main' de la partie.
*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>


#include "partie.h"

int partie()
{

	/* CREATION DES VARIABLES */
	int joueur, i = 0, encombrement_pion, coups = 0, quitter=0;
	int mode_jeu;  //IA ou VS
	coord case_ini, case_fin; //structures.h

    /* CREATION DES DONNEES DE LA SDL */
	SDL_Surface* tableau_surface[NB_SURFACE]; // Tableau de surfaces
	SDL_Rect tableau_position[NB_POS]; // Tableau de positions
	initialiser_SDL(tableau_surface, tableau_position);


	/*
		La grille de jeu, sous forme de tableau bi-dimensionnel
		S_case est d�fini dans structures.h
	*/
	S_case plateau_jeu[TAILLE_PLAT][TAILLE_PLAT];
	S_masque masque[MASQUE_MAX][MASQUE_MAX];
	coord tab_points_clefs[DIM_TAB_POINTS_CLEFS];


	/*
		La liste des globules. Elle doit servir pour l'IA.
		S_pion est d�fini dans structures.h
	*/

	S_pion* tab_listes[DIM_TAB_LISTES];
		tab_listes[GLOBULE] = NULL;
		tab_listes[VIRUS] = NULL;
		tab_listes[GR] = NULL;


	/*
		G�n�re le plateau et la liste
		Le plateau est rempli � partir d'un fichier texte selon des r�gles pr�d�finies
		La liste est remplie au m�me moment.
	*/
    generer_plateau(plateau_jeu, tab_listes, tab_points_clefs); //extraction_fichier.h

	/*
		Le premier joueur est la d�fense
	*/
	joueur = GLOBULE;
    /*
        On demande � l'utilisateur quel type de partie veut-il faire ? Humain VS Humain ou Humain VS IA.
    */
	mode_jeu=selection_mode_jeu(tableau_surface);



	do
	{
		/*
			Introduire les changements:
				-faire entrer des virus
				-gonfler les globules
				-injecter les globules rouges
		*/


		afficher_joueur(joueur, tableau_surface, tableau_position);
		afficher_toto(tableau_surface, tableau_position);



		if(tab_listes[joueur] != NULL)
		{
		    if(mode_jeu == IA && joueur == VIRUS)
             /*
			On laisse l'IA g�rer les d�placements de ses pions.
            */
            {
                meilleur_coup(plateau_jeu, tab_listes, tab_points_clefs, &case_ini, &case_fin);
                fprintf(stderr, "YO");
                masque_prgm(masque, plateau_jeu[case_ini.x][case_ini.y].pion, plateau_jeu); // Utile au chemin.
                fprintf(stderr, "YA");

                SDL_Delay(400); // Pour s�parer les coups de l'IA
            }
			else
            /*
			Demander les coordonn�es
			V�rifier que les coordonn�es saisies sont valides
			Demander la destination
			V�rifier que les coordonn�es de destination saisies sont valides
            */

			{
                do{
                    afficher_plateau(plateau_jeu, tableau_surface, tableau_position, case_ini, 0);
                    SDL_Flip(tableau_surface[ECRAN]);

                    do demander_coord(&case_ini, 1, tableau_surface, tableau_position, &quitter); // L'utilisateur doit cliquer sur le pion qu'il souhaite d�placer
                    while(verif_coord(plateau_jeu, case_ini, joueur)==0);

                    //Attention, comme un globule peut occuper plusieurs cases,
                    //il faut bien s�lectionner la case caract�ristique (la plus proche de l'origine)
                    if(joueur==GLOBULE)
                    {
                        case_ini = case_caract(plateau_jeu, case_ini);
                        encombrement_pion = plateau_jeu[case_ini.x][case_ini.y].pion->niveau;
                    }
                    else
                        encombrement_pion = 1;

                    masque_prgm(masque, plateau_jeu[case_ini.x][case_ini.y].pion, plateau_jeu);
                    afficher_masque(masque,tableau_surface, tableau_position, case_ini);

                    demander_coord(&case_fin, 2, tableau_surface, tableau_position, &quitter); // L'utilisateur doit cliquer sur l'emplacement o� il souhaite d�placer le pion
                }while(is_valid(plateau_jeu, masque, case_ini, case_fin, joueur, encombrement_pion, tab_points_clefs[CONTROLE])==0);

			}

            afficher_chemin(masque, plateau_jeu, case_ini, case_fin, tableau_surface, tableau_position); // Affiche pixel par pixel la progression du pion d�plac�
			deplacement(plateau_jeu, tab_listes, case_ini, case_fin, tab_points_clefs[CONTROLE]);//D�placer le pion, g�re les attaques, les divisions de globules blancs etc.
		}

        if(joueur == GLOBULE)
            entree_pions(plateau_jeu, tab_listes, tab_points_clefs[ENTREE_VIRUS], VIRUS); // On fait entrer des virus

        deplacer_gr(plateau_jeu, tab_listes[GR]); // Deplacer le globule rouge de fa�on al�atoire

        i++;

		if((joueur == GLOBULE && i%5 == 0) || (joueur == VIRUS && i%9 == 0)) // On rentre dans la condition tout les 5 tours pour les globules blancs et tout les 9 tours pour les virus
		{
            i=0;
            coups++;
		    evolution_globules(plateau_jeu, tab_listes[GLOBULE]);
            changer_joueur(&joueur);
            entree_pions(plateau_jeu, tab_listes, tab_points_clefs[ENTREE_GR], GR); // On fait entrer des globules rouges
		}

    /*
        On continue la partie tant qu'il reste des globules ou des virus, que l'utilisateur ne veut pas quitter, qu'il n'y a pas assez de virus dans
        le point de contr�le et que le nombre de tours d�fini pour la victoire des globules blancs n'est pas atteint!
    */
	}while(quitter==0 && tab_listes[GLOBULE] != NULL && tab_listes[VIRUS] != NULL && cpt_controle(0)<COUPS_CONTROLE && coups<COUPS_VICTOIRE_GB);

	afficher_vainqueur(tab_listes, tableau_surface, coups);
	cpt_controle(2); // Remise � z�ro du compteur

    liberer_SDL(tableau_surface);
    return 0;
}


