/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : affichage_sdl.cpp
    Dernière Modification : 16/05/2010

    Description :
        Contient toutes les fonctions et procédures nécessaire à l'affichage de la SDL de partie()
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "structures.h"

#include <SDL/SDL.h>

#ifdef WINDOWS
    #include <SDL/SDL_ttf.h>
    #include <FMOD/fmod.h>
#endif

#ifdef MAC
    #include <SDL_ttf/SDL_ttf.h>
    #include "fmod.h"
    #include "fmod_errors.h"
    #include "wincompat.h"
#endif

#include "affichage_sdl.h"




void demander_coord(coord* case_a_jouer, int choix, SDL_Surface* tableau_surface[], SDL_Rect tableau_position[], int *quitter)
/* PS : case_a_jouer, quitter
    Aquiert les coordonnées de la case de départ ou d'arrivée selon si choix vaut 0 ou 1, modifie quitter si l'utilisateur clique sur la croix de "fermer"
*/
{
	int continuer = 1;
    SDL_Event event;


	switch(choix)
	{
		case 1: // Cas où l'on veut la case de départ
			while(continuer)
			{
			    SDL_WaitEvent(&event); // Attend qu'un évenement se produise

			    switch(event.type)
                {
                    case SDL_QUIT: // Si l'utilisateur clique sur "fermer"
                        *quitter=1;
                        exit(EXIT_SUCCESS);
                        break;

                    case SDL_MOUSEBUTTONDOWN : // Si l'utilisateur clique bas
                        if((event.button.x>=POS_PLATEAU_X+1 && event.button.x<=POS_PLATEAU_X+601) && (event.button.y>=POS_PLATEAU_Y && event.button.y<=POS_PLATEAU_Y+601)) // Si l'utilisateur a cliquer dans le plateau de jeu
                        {
                           continuer=0;
                           case_a_jouer->x=sdl2cons(event.button.x,'h'); // Conversion des pixels en unité de tableau
                           case_a_jouer->y=sdl2cons(event.button.y,'v');
                        }
                        break;
                }
			}
        break;

		default: // Cas où l'on veut la case d'arrivée (même principe que ci-dessus)
			while(continuer)
			{
			    SDL_WaitEvent(&event);
			    switch(event.type)
			    {
			        case SDL_QUIT:
                        exit(EXIT_SUCCESS);
                        break;

                    case SDL_MOUSEBUTTONDOWN:
                        if((event.button.x>=POS_PLATEAU_X+1 && event.button.x<=POS_PLATEAU_X+601) && (event.button.y>=POS_PLATEAU_Y && event.button.y<=POS_PLATEAU_Y+601))
                        {
                            continuer=0;
                            case_a_jouer->x=sdl2cons(event.button.x, 'h');
                            case_a_jouer->y=sdl2cons(event.button.y, 'v');
                        }
                        break;
			    }
			}
        break;
	}

}

void afficher_joueur(const int joueur, SDL_Surface* tableau_surface[], SDL_Rect tableau_position[])
/* PS : /
    Affiche le joueur actuel dans l'en-tête (équipe + image) ou l'en-tête de l'éditeur.
*/
{
    tableau_position[0].x=0; // Définition de la position de l'en-tête
    tableau_position[0].y=0;

    switch(joueur)
	{
		case GLOBULE:
            SDL_BlitSurface(tableau_surface[JOUEUR_GB], NULL, tableau_surface[EN_TETE], &tableau_position[0]); // Collage de l'image joueur_gb sur l'en-tête
			break;
		case VIRUS:
			SDL_BlitSurface(tableau_surface[JOUEUR_VIR], NULL, tableau_surface[EN_TETE], &tableau_position[0]); // Collage de l'image joueur_vir sur l'en-tête
			break;
        default :
            SDL_BlitSurface(tableau_surface[BUILDER], NULL, tableau_surface[EN_TETE], &tableau_position[0]); // Collage de l'image en_tete_builder sur l'en-tête
            break;

	}

	SDL_BlitSurface(tableau_surface[EN_TETE], NULL, tableau_surface[ECRAN], &tableau_position[0]);  // Collage de l'en-tête sur l'écran
}


void afficher_plateau(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], SDL_Surface* tableau_surface[], SDL_Rect tableau_position[], coord coord_actuel, int choix)
/* PS : /
    Affiche la grille de jeu et les pions qui y figurent, choix permet d'afficher ou non le pion que l'on est en train de jouer. Celà permet de ne pas avoir l'affichage
    de la position initial du pion lors de son déplacement graphique avec la fonction de chemin.
*/
{
	int pion,i,j; // Nécessaire au parcours du tableau
	coord coordonnees; // Nécessaire à l'utilisation de certaines fonctions

	tableau_position[0].x=0;  // Définition de la position de collage
	tableau_position[0].y=0;
	SDL_BlitSurface(tableau_surface[FOND_ECRAN], NULL, tableau_surface[PLATEAU], &tableau_position[0]); // Collage de l'image de fond d'écran sur le plateau

    /* AFFICHAGE DES PIONS DU PLATEAU DE JEU */
	for(j=0; j<TAILLE_PLAT; j++)
	{
	   for(i=0; i<TAILLE_PLAT; i++)
	   {
	       if(choix == 0 || (choix==1 && coord_actuel.x!=i || coord_actuel.y!=j)) // Si l'on veut afficher la totalité du plateau, ou si les coordonnes de la case actuel ne sont pas celles de la position initiale du pion
	       {
                coordonnees.x=i;
                coordonnees.y=j;

                tableau_position[0].x=cons2sdl(i);
                tableau_position[0].y=cons2sdl(j);

                pion=-1;

                pion=contenu_case(coordonnees,plateau);
                if(pion!=-1) // S'il y a quelque chose dans la case
                    SDL_BlitSurface(tableau_surface[pion], NULL, tableau_surface[PLATEAU], &tableau_position[0]);
	       }
	   }
	}

	SDL_BlitSurface(tableau_surface[PLATEAU], NULL, tableau_surface[ECRAN], &tableau_position[PLATEAU]); // Collage du plateau sur l'écran
}

void afficher_masque(S_masque masque[MASQUE_MAX][MASQUE_MAX],SDL_Surface* tableau_surface[], SDL_Rect tableau_position[], coord case_actuelle)
/* PS : /
    Affiche le masque, c'est à dire les déplacements autorisé en surbrillance
*/
{
    int i,j; // Nécessaire au parcours du tableau
    for(j=0;j<MASQUE_MAX;j++)
    {
        for(i=0;i<MASQUE_MAX;i++)
            {
				if(masque[i][j].poids>0) // Si la case est atteignable (exclu la case initiale du déplacement du pion)
				{
				    tableau_position[0].x=cons2sdl(case_actuelle.x-VITESSE_MAX+i); // On doit convertir les coordonées du tableau en coordonnées de pixels
				    tableau_position[0].y=cons2sdl(case_actuelle.y-VITESSE_MAX+j);
				    SDL_BlitSurface(tableau_surface[MASQUE], NULL, tableau_surface[PLATEAU], &tableau_position[0]); // Collage de la surbrillance sur le plateau
				}
			}
	}

	SDL_BlitSurface(tableau_surface[PLATEAU], NULL, tableau_surface[ECRAN], &tableau_position[PLATEAU]); // Collage du plateau sur l'écran
	SDL_Flip(tableau_surface[ECRAN]); // Mise à jour de l'écran
}


int selection_mode_jeu(SDL_Surface* tableau_surface[])
/* PS : mode_de_jeu (Humain VS ia ou Humain VS Humain)
    Affiche un écran permettant de sélectioner le mode de jeu désiré.
    Cette fonction est isolée des autres et ne sert qu'une seule fois. Elle a donc un tableau de surface propre.
*/
{
    int i, fond=0, ia=1, human=2, continuer=2;

    SDL_Surface* tableau_s[3];             // Création du tableau contenant les surfaces
    SDL_Rect pos_fond, pos_ia, pos_human;   // Création des position
    SDL_Event event;                        // Création d'un évenement
    FSOUND_SAMPLE *robot_voice = NULL;      // Création d'un son

    /* INITIALISATION DES IMAGES ET SON */
    TTF_Font *police = NULL;                // Définition de la police
    police = TTF_OpenFont("polices/police_menu.ttf", 28);
    SDL_Color blanc = {255,255,255};

    tableau_s[fond] = SDL_LoadBMP("pic/fonds/fond_mode_jeu.bmp");
    tableau_s[ia] = TTF_RenderText_Blended(police, "Humain VS Ordinateur",blanc);
    tableau_s[human] = TTF_RenderText_Blended(police, "Humain VS Humain",blanc);

    robot_voice = FSOUND_Sample_Load(FSOUND_FREE, "music/ready.mp3", 0, 0, 0);


    /* COLLAGE DES IMAGES ET TEXTES AU BON EMPLACEMENT */
    pos_fond.x=0;
    pos_fond.y=0;
    SDL_BlitSurface(tableau_s[fond], NULL, tableau_surface[ECRAN], &pos_fond);

    pos_ia.x=(LARGEUR_FENETRE-tableau_s[ia]->w)/2;
    pos_ia.y=100;
    SDL_BlitSurface(tableau_s[ia], NULL, tableau_surface[ECRAN], &pos_ia);


    pos_human.x=(LARGEUR_FENETRE-tableau_s[human]->w)/2;
    pos_human.y=200;
    SDL_BlitSurface(tableau_s[human], NULL, tableau_surface[ECRAN], &pos_human);

    SDL_Flip(tableau_surface[ECRAN]);


    while (continuer==2)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer =-1;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if((event.button.x>=pos_ia.x && event.button.x<=pos_ia.x+tableau_s[ia]->w) && (event.button.y>=pos_ia.y && event.button.y<=pos_ia.y+tableau_s[ia]->h)) // Si l'utilisateur clique sur "Humain VS IA"
                    {
                        continuer=IA;
                        FSOUND_PlaySound(FSOUND_FREE, robot_voice); // Jouer le son
                        SDL_Delay(500); // Attendre le temps que le son se finisse
                    }

                else if((event.button.x>=pos_human.x && event.button.x<=pos_human.x+tableau_s[human]->w) && (event.button.y>=pos_human.y && event.button.y<=pos_human.y+tableau_s[human]->h)) // Si l'utilisateur clique sur "Humain VS IA"
                    continuer=VS;
                break;
        }
    }

    /* LIBERATION DES SURFACES ET SON */
    for(i=0;i<3;i++)
        SDL_FreeSurface(tableau_s[i]);
    FSOUND_Sample_Free(robot_voice);

    SDL_FillRect(tableau_surface[ECRAN], NULL, SDL_MapRGB(tableau_surface[ECRAN]->format, 100, 100, 100)); // On efface l'écran.


    return continuer; // retourne IA ou VS
}

void afficher_chemin(S_masque masque[MASQUE_MAX][MASQUE_MAX], S_case plateau[TAILLE_PLAT][TAILLE_PLAT], coord case_ini, coord case_fin, SDL_Surface* tableau_surface[], SDL_Rect tableau_position[])
/* PS : /
    Affiche le chemin parcouru par un pion lorsqu'il se déplace de case_ini à case_fin
*/
{
    S_chemin* L_chemin=NULL;
    S_chemin* parcours=NULL; // Sert au parcours de la liste
    SDL_Rect pos, pos_fond;
    SDL_Surface* pion,* fond;
    int i;

    pion = tableau_surface[contenu_case(case_ini, plateau)]; // pion devient la surface du pion que l'on veut déplacer
    afficher_plateau(plateau, tableau_surface, tableau_position, case_ini, 1);
    fond = tableau_surface[PLATEAU]; // Stockage de l'image du plateau précèdement affiché pour ne pas executer afficher_plateau() (qui est très couteux) plein de fois

    pos_fond.x=POS_PLATEAU_X;
    pos_fond.y=POS_PLATEAU_Y;

    chemin(masque, &L_chemin, case_ini, case_fin); // Création de la liste de coordonnées du chemin

    parcours=L_chemin; // Initialisation de 'parcours' au premier élèment de la liste

    while(parcours->suiv!=NULL) // Tant qu'il reste un élèment après l'actuel
    {
        pos.x=POS_PLATEAU_X + cons2sdl(parcours->etape.x); // Définition de la position de l'image à coller au début de la case actuelle de la liste
        pos.y=POS_PLATEAU_Y + cons2sdl(parcours->etape.y);

        /* Pour un déplacement plus fluide, on avance pixel par pixel tant que l'on est dans la case actuelle définie par ci-dessus */
        for(i=0;i<20;i++) // Tant que l'on est dans les coordonnées de l'élèment actuel de la liste
        {
            if(parcours->suiv->etape.x==parcours->etape.x+1)
               pos.x++;
            else if(parcours->suiv->etape.x==parcours->etape.x-1)
                pos.x--;
            if(parcours->suiv->etape.y==parcours->etape.y+1)
                pos.y++;
            else if(parcours->suiv->etape.y==parcours->etape.y-1)
                pos.y--;
            SDL_BlitSurface(fond, NULL, tableau_surface[ECRAN], &pos_fond);  // Collage du plateau
            SDL_BlitSurface(pion, NULL, tableau_surface[ECRAN], &pos); // Collage de l'image du pion
            SDL_Flip(tableau_surface[ECRAN]); // Màj de l'écran
            SDL_Delay(5); // Attend ~15ms (granularité du temps dans le CPU) pour que l'affichage ne se fasse pas trop vite
        }


    parcours=parcours->suiv; // Passage à l'élèment suivant de la liste
    }
}

int contenu_case(coord coordonnees, S_case plateau[TAILLE_PLAT][TAILLE_PLAT])
/* PS : retour (qui prend la valeur de la constante correspondant au contenu de la case).
    Permet de savoir quel pion se trouve dans une case du tableau de jeu.
*/
{
    int retour, i, j;
    i=coordonnees.x;
    j=coordonnees.y;

    switch(plateau[i][j].type)
    {

        case NORMALE:

            if(plateau[i][j].pion!=NULL) // S'il y a un pion dans cette case
            {
                if((plateau[i][j].pion)->type_pion == GLOBULE && coord_eq(case_caract(plateau,coordonnees),coordonnees)) // Si c'est un globule blanc et s'il s'agit de sa case caractéristique
                {
                    switch((plateau[i][j].pion)->vitesse) // Différentes couleurs selon la vitesse du globule blanc
                    {
                        case 0: // Si le globule blanc est paralysé
                                switch((plateau[i][j].pion)->niveau) // Affichage du pion selon son niveau
                                {
                                    case 1:
                                            retour =  GB1_1; break;
                                    case 2:
                                            retour =  GB2_1; break;
                                    case 3:
                                            retour =  GB3_1; break;
                                }
                            break;

                        case VITESSE_INI_GB: // Si le globule blanc a sa vitesse maximum
                                switch((plateau[i][j].pion)->niveau) // Affichage du pion selon son niveau
                                {
                                    case 1:
                                            retour =  GB1_3; break;
                                    case 2:
                                            retour =  GB2_3; break;
                                    case 3:
                                            retour =  GB3_3; break;
                                }
                            break;
                        default: // Si sa vitesse est comprise ente les deux
                                switch((plateau[i][j].pion)->niveau) // Affichage du pion selon son niveau
                                {
                                    case 1:
                                            retour =  GB1_2; break;
                                    case 2:
                                            retour =  GB2_2; break;
                                    case 3:
                                            retour =  GB3_2; break;
                                }
                            break;
                    }
                }
                else if((plateau[i][j].pion)->type_pion == VIRUS) // Si le pion est un virus
                    {
                        switch((plateau[i][j].pion)->niveau) // Affichage du pion selon sa facultée
                        {
                            case 1 :
                                retour =  VIR1; break;
                            case 2 :
                                retour =  VIR2; break;
                            case 3 :
                                retour =  VIR3; break;
                        }
                    }

                else if((plateau[i][j].pion)->type_pion == GR) // Sinon, c'est forcement un globule rouge
                    {
                        srand(time(0));
                        retour = rand()%2 == 0 ? GROUGE1 : GROUGE2;
                    }

                else // Si la case n'est pas la case caractéristique du globule blanc
                    retour=-1;

                break;
            }
            else // S'il n'y a rien dans la case
                retour = -1;
            break;

        case BLOQUEE: // Si c'est une case bloquée
            retour =  BLOQ;
            break;
        case CONTROLE: // Si c'est un point de contrôle
            retour =  CTRL;
            break;
        case ENTREE_VIRUS: //  Si c'est un point d'entrée de virus
            retour =  E_VIRUS;
            break;
        case ENTREE_GR: // Si c'est un point d'entrée de globule rouge
            retour =  E_GR;
            break;
        default :
            retour=-1;
            break;
    }

    return retour;
}

void afficher_vainqueur(S_pion* tab_listes[DIM_TAB_LISTES], SDL_Surface* tableau_surface[], int coups)
/* PS : /
    Affiche le vainqueur à la fin de la partie
*/
{   SDL_Surface* fond;
    SDL_Rect pos;
    SDL_Event event;
    int continuer=1;

    pos.x=0;
    pos.y=0;


    if(tab_listes[GLOBULE]==NULL || cpt_controle(0)>=COUPS_CONTROLE) // Si les virus gagnent
        {fond = SDL_LoadBMP("pic/fonds/virus_win.bmp"); // Chargement de l'image du vainqueur
        fprintf(stderr,"\nvictoire virus OK");}

    else if(tab_listes[VIRUS]==NULL || coups>=COUPS_VICTOIRE_GB)
        {fond = SDL_LoadBMP("pic/fonds/gb_win.bmp");
        fprintf(stderr,"\nvictoire gb OK");}
    else
        fprintf(stderr, "Erreur : Aucun gagnant");

    SDL_BlitSurface(fond, NULL, tableau_surface[ECRAN], &pos);
    SDL_Flip(tableau_surface[ECRAN]);

    /* ATTEND QUE L'UTILISATEUR CHOISISSE DE PASSER */

    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN :
                if(event.key.keysym.sym==SDLK_ESCAPE)
                    continuer=0;
            break;
        }
    }

    SDL_FreeSurface(fond); // Libération de la surface
}




void afficher_toto(SDL_Surface* tableau_surface[], SDL_Rect tableau_position[])
{




    switch(cpt_controle(0))
    {
        case 0:
        SDL_BlitSurface(tableau_surface[TOTO1], NULL, tableau_surface[ECRAN], &tableau_position[TOTO]);
        break;

        case 1:
        SDL_BlitSurface(tableau_surface[TOTO2], NULL, tableau_surface[ECRAN], &tableau_position[TOTO]);
        break;

        case 2:
        SDL_BlitSurface(tableau_surface[TOTO3], NULL, tableau_surface[ECRAN], &tableau_position[TOTO]);
        break;

        case 3:
        SDL_BlitSurface(tableau_surface[TOTO4], NULL, tableau_surface[ECRAN], &tableau_position[TOTO]);
        break;

    }
}










