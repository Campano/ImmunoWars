/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : affichage_sdl.h
    Dernière Modification : 16/05/2010

    Description :
        Header
*/
#ifndef AFFICHAGE_SDL
	#define AFFICHAGE_SDL

	#include "structures.h"
	#include "jeu.h"
	#include "outils_sdl.h"
	#include "masque.h"
	#include "tableau_jeu.h"


    #define NB_SURFACE 32
    #define NB_POS 4
    #define NB_CHAR 11
    #define RESOLUTION 32

    #define LARGEUR_FENETRE 620
    #define HAUTEUR_FENETRE 730

    #define POS_PLATEAU_X 9
    #define POS_PLATEAU_Y 124


    /*
        DEFINITION DES CONSTANTES UTILES AU PARCOURS DES TABLEAU DE SURFACE ET DE POSITIONS
    */
    #define ECRAN 0
    #define EN_TETE 1
    #define PLATEAU 2
    #define QUADRILLAGE_H 3
    #define QUADRILLAGE_V 4

    #define GB1_1 5
    #define GB1_2 6
    #define GB1_3 7
    #define GB2_1 8
    #define GB2_2 9
    #define GB2_3 10
    #define GB3_1 11
    #define GB3_2 12
    #define GB3_3 13
    #define VIR1 14
    #define VIR2 15
    #define VIR3 16
    #define E_VIRUS 17
    #define E_GR 18
    #define BLOQ 19
    #define CTRL 20
    #define GROUGE1 21
    #define GROUGE2 22

    #define JOUEUR_GB 23
    #define JOUEUR_VIR 24

    #define MASQUE 25
    #define FOND_ECRAN 26
    #define BUILDER 27

    #define TOTO 3
    #define TOTO1 28
    #define TOTO2 29
    #define TOTO3 30
    #define TOTO4 31



	void afficher_plateau(S_case plateau[TAILLE_PLAT][TAILLE_PLAT], SDL_Surface* tableau_surface[], SDL_Rect tableau_position[], coord coord_actuel, int choix);
	int contenu_case(coord coordonnees, S_case plateau[TAILLE_PLAT][TAILLE_PLAT]);

	void afficher_joueur(const int joueur, SDL_Surface* tableau_surface[], SDL_Rect tableau_position[]);
	void demander_coord(coord* case_a_jouer, int choix, SDL_Surface* tableau_surface[], SDL_Rect tableau_position[], int *quitter);
	void afficher_masque(S_masque masque[MASQUE_MAX][MASQUE_MAX],SDL_Surface* tableau_surface[], SDL_Rect tableau_position[], coord case_actuelle);

	int selection_mode_jeu(SDL_Surface* tableau_surface[]);

	void afficher_chemin(S_masque masque[MASQUE_MAX][MASQUE_MAX], S_case plateau[TAILLE_PLAT][TAILLE_PLAT], coord case_ini, coord case_fin, SDL_Surface* tableau_surface[], SDL_Rect tableau_position[]);
    void afficher_vainqueur(S_pion* tab_listes[DIM_TAB_LISTES], SDL_Surface* tableau_surface[], int coups);

    void afficher_toto(SDL_Surface* tableau_surface[], SDL_Rect tableau_position[]);

#endif
