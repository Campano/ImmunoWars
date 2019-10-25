/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : editeur.cpp
    Dernière Modification : 16/05/2010

    Description :
        Contient toutes les fonctions et procédures nécessaire à l'editeur et l'éditeur lui même
*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

#include"editeur.h"

void editeur_prg()
/* PS :/
    Editeur
*/
{   /* DECLARATIONS */
    S_case tableau[TAILLE_PLAT][TAILLE_PLAT];

    SDL_Surface* tableau_surface[NB_SURFACE], *temp;
	SDL_Rect tableau_position[NB_POS], coordonnees;
	coord a; // Doit être passé en argument pour certaines fonctions

	int nb_entree_gr=0, nb_entree_virus=0, nb_pt_controle=0;
    int continuer=1, continuer1=1, choix, quit=0;
    SDL_Event event;


	/* INITIALISATIONS */
	initialiser_plateau(tableau);
	initialiser_SDL(tableau_surface, tableau_position);


    /* AFFICHAGE */
    afficher_joueur(BUILDER, tableau_surface, tableau_position);
    afficher_plateau(tableau, tableau_surface, tableau_position, a, 0);

    SDL_Flip(tableau_surface[ECRAN]);




    while(!quit)
    {
        SDL_ShowCursor(SDL_ENABLE); // Affiche le curseur

        afficher_joueur(BUILDER, tableau_surface, tableau_position);
        afficher_plateau(tableau, tableau_surface, tableau_position, a, 0);
        temp=tableau_surface[PLATEAU]; // On creer une copie du plateau pour ne pas avoir a reparcourir le tableau pour l'afficher

        /* REINITIALISATION DES VARIABLES */
        choix=0;
        continuer=1;
        continuer1=1;

        while (continuer)
        {
            SDL_WaitEvent(&event);
            switch(event.type)
            {
                case SDL_QUIT: // Si le joueur ferme, la map n'est pas sauvegardée
                    quit=1;
                    continuer=0;
                    break;
                case SDL_KEYDOWN: // S'il appui sur une touche
                    switch(event.key.keysym.sym)
                    {
                        /*
                            Selon la touche sur laquelle il appui, on selectione tel ou tel pion
                        */
                        case SDLK_1:
                            choix=GB1_3; break;
                        case SDLK_2:
                            choix=VIR1; break;
                        case SDLK_3:
                            choix=BLOQ; break;
                        case SDLK_4:
                            if(!nb_entree_gr)
                                choix=E_GR;
                            nb_entree_gr++;
                            break;
                        case SDLK_5:
                            if(!nb_entree_virus)
                                choix=E_VIRUS;
                            nb_entree_virus++;
                            break;
                        case SDLK_6:
                            if(!nb_pt_controle)
                                choix=CTRL;
                            nb_pt_controle++;
                            break;
                        case SDLK_ESCAPE:
                            continuer=0;
                            quit=1; break;
                        default : break;
                    }
                    if(choix!=0)
                        continuer=0; // Pour sortir de la boucle
                    break;
            }
        }

        SDL_ShowCursor(SDL_DISABLE); // On enlève le curseur

        if(!quit)
        {

            while(continuer1)
            {
                /*
                    On affiche le pion séléctioné qui suit la souris, puis se fixe sur l'écran
                */

                SDL_WaitEvent(&event);
                switch(event.type)
                {
                    case SDL_MOUSEMOTION:
                        if(event.motion.y>=POS_PLATEAU_Y  + tableau_surface[choix]->h/2 && event.motion.y<=POS_PLATEAU_Y+tableau_surface[PLATEAU]->h- tableau_surface[choix]->h/2&& event.motion.x>=POS_PLATEAU_X +tableau_surface[choix]->w/2&& event.motion.x<=POS_PLATEAU_X+tableau_surface[PLATEAU]->w-tableau_surface[choix]->w/2)
                           {
                            coordonnees.x = event.motion.x - tableau_surface[choix]->w/2;
                            coordonnees.y = event.motion.y - tableau_surface[choix]->h/2;

                            SDL_BlitSurface(temp, NULL, tableau_surface[ECRAN], &tableau_position[PLATEAU]);
                            SDL_BlitSurface(tableau_surface[choix], NULL, tableau_surface[ECRAN], &coordonnees);
                            SDL_Flip(tableau_surface[ECRAN]);
                           }
                        break;

                    case SDL_MOUSEBUTTONUP :
                        if(event.button.y>=tableau_surface[EN_TETE]->h && event.button.y<=POS_PLATEAU_Y+tableau_surface[PLATEAU]->h && event.button.x>=POS_PLATEAU_X && event.button.x<=POS_PLATEAU_X+tableau_surface[EN_TETE]->w)
                        {
                        continuer1=0;
                        coordonnees.x = event.button.x - tableau_surface[choix]->w/2;
                        coordonnees.y = event.button.y - tableau_surface[choix]->h/2;

                        SDL_BlitSurface(temp, NULL, tableau_surface[ECRAN], &tableau_position[PLATEAU]);
                        SDL_BlitSurface(tableau_surface[choix], NULL, tableau_surface[ECRAN], &coordonnees);
                        SDL_Flip(tableau_surface[ECRAN]);
                        }
                        break;
                }
            }
                // Conversion des coordonnees pour les rentrer dans le tableau
                coordonnees.x=sdl2cons(coordonnees.x + tableau_surface[choix]->w/2, 'h');
                coordonnees.y=sdl2cons(coordonnees.y + tableau_surface[choix]->h/2, 'v');

                switch(choix)
                {
                    case GB1_3:
                        /*
                            On doit redéfinir le niveau et la vitesse pour le globule blanc et le virus pour que la fonction d'affichage fonctionne
                        */
                        tableau[coordonnees.x][coordonnees.y].type=NORMALE;
                        tableau[coordonnees.x][coordonnees.y].pion = (S_pion*) malloc(sizeof (S_pion));
                        tableau[coordonnees.x][coordonnees.y].pion->type_pion=GLOBULE;
                        tableau[coordonnees.x][coordonnees.y].pion->vitesse=VITESSE_INI_GB;
                        tableau[coordonnees.x][coordonnees.y].pion->niveau=1;
                        tableau[coordonnees.x][coordonnees.y].pion->pos.x=coordonnees.x;
                        tableau[coordonnees.x][coordonnees.y].pion->pos.y=coordonnees.y;
                        break;
                    case VIR1:
                        tableau[coordonnees.x][coordonnees.y].type=NORMALE;
                        tableau[coordonnees.x][coordonnees.y].pion = (S_pion*) malloc(sizeof (S_pion));
                        tableau[coordonnees.x][coordonnees.y].pion->type_pion=VIRUS;
                        tableau[coordonnees.x][coordonnees.y].pion->vitesse=VITESSE_INI_VIRUS;
                        tableau[coordonnees.x][coordonnees.y].pion->niveau=1;
                        break;
                    case BLOQ:
                        tableau[coordonnees.x][coordonnees.y].type=BLOQUEE;
                        break;
                    case E_GR:
                        tableau[coordonnees.x][coordonnees.y].type=ENTREE_GR;
                        break;
                    case E_VIRUS:
                        tableau[coordonnees.x][coordonnees.y].type=ENTREE_VIRUS;
                        break;
                    case CTRL:
                        tableau[coordonnees.x][coordonnees.y].type=CONTROLE;
                        break;
                }
        }
    }

        sauvegarde(tableau); // Quand le joueur a décidé de quitter , on sauvegarde le plateau



}

void initialiser_plateau(S_case plateau[TAILLE_PLAT][TAILLE_PLAT])
/* PS : /
    Initialise le plateau de jeu
*/
{
    int i, j;

    	for(i=0; i<TAILLE_PLAT; i++)
        {
            for(j=0; j<TAILLE_PLAT; j++)
            {
                plateau[i][j].pion = NULL;
                plateau[i][j].type = NORMALE;
            }
        }
}

void sauvegarde(S_case plateau[TAILLE_PLAT][TAILLE_PLAT])
/* PS :/
    Sauvegarde la plateau de jeu de l'éditeur
*/
{
    /*
        DECLARATIONS
    */
	int i, j;
	S_pion* pion;
    S_chemin *L_gb = NULL, *L_vir = NULL, *L_blok = NULL, *L_evir = NULL, *L_egr = NULL, *L_ctrl = NULL;


	/*
        Pour chaque case, on distingue chaque pion
    */
	for(j=0; j<TAILLE_PLAT; j++)
		for(i=0; i<TAILLE_PLAT; i++)
		{
			switch(plateau[i][j].type)
			{
				case BLOQUEE : new_coord(&L_blok, i, j); break;
				case CONTROLE : new_coord(&L_ctrl, i, j); break;
				case ENTREE_VIRUS : new_coord(&L_evir, i, j); break;
				case ENTREE_GR : new_coord(&L_egr, i, j); break;
				default :
                    pion = plateau[i][j].pion;
					if(pion != NULL)
					{
						switch(pion->type_pion)
						{
							case GLOBULE : new_coord(&L_gb, i, j); break;
							case VIRUS : new_coord(&L_vir, i, j); break;
						}
					}
					break;
			}
		}

	/*
        Ouverture du fichier texte
    */
	FILE* fichier = NULL;
    fichier = fopen("map/map.txt", "w+");

	/*
        Impression des caractères repères et des listes de coordonnées
    */
	if(fichier!=NULL)
	{
		fprintf(fichier, "FICHIER DE SAUVEGARDE\n#");
		fprintf(fichier, "Globules Blancs : ");
		fprintCoord(fichier, L_gb);
		fprintf(fichier, "Virus : ");
		fprintCoord(fichier, L_vir);
		fprintf(fichier, "Cases Bloquees : ");
		fprintCoord(fichier, L_blok);
		fprintf(fichier, "Points de controle : ");
		fprintCoord(fichier, L_ctrl);
		fprintf(fichier, "Entree Virus : ");
		fprintCoord(fichier, L_evir);
		fprintf(fichier, "Entree GR: ");
		fprintCoord(fichier, L_egr);

	}

	fclose(fichier);

    /*
        LIBERATION MEMOIRE
    */
	suppr_elt(L_gb);
	suppr_elt(L_vir);
	suppr_elt(L_blok);
	suppr_elt(L_evir);
	suppr_elt(L_egr);
	suppr_elt(L_ctrl);
}

void fprintCoord(FILE* fichier, S_chemin* L_coord)
/* PS : /
    Imprime la liste des coord d'une liste fixé dans le .txt
*/
{
	while(L_coord != NULL)
	{
		fprintf(fichier, "(%d;%d) ", L_coord->etape.x + 1, L_coord->etape.y + 1);
		L_coord = L_coord->suiv;
	}
	fprintf(fichier, "\n#\n");
}

void suppr_elt(S_chemin* element)
/* PS : /
    Supprime les elements
*/
{
	if(element != NULL)
	{
		suppr_elt(element->suiv);
		free(element);
	}
}

void new_coord(S_chemin** L_deb, int x, int y)
/* PS : L_deb
    Créé une nouvelle VD et la chaine avec les précèdentes
*/
{
	S_chemin* actuel;
	int i=0;

	S_chemin* element = (S_chemin*) malloc(sizeof(S_chemin));
		element->etape.x = x;
		element->etape.y = y;
		element->suiv = NULL;

	if(*L_deb == NULL)
		*L_deb = element;
	else
	{
		actuel = *L_deb;
		while(actuel->suiv != NULL)
		{
		    fprintf(stderr, "#%d\t%d\t%d", i, actuel, actuel->suiv);
			actuel = actuel->suiv;
		}
		actuel->suiv = element;
	}

}

