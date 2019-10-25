/*
    Projet second semestre EPF 1A, Promo 2014
    Nom du projet : IMMUNO WARS
    Auteurs : Simon CAMPANO & Nicolas MANIE

    Nom du fichier : strucutres.h
    Dernière Modification : 16/05/2010

    Description :
        Header
        Contient une bonne partie des structures de données ainsi que les constantes utiles au jeu
*/

#ifndef STRUCTURES
	#define STRUCTURES

	#define WINDOWS

	//PLATEAU DE JEU
	#define TAILLE_PLAT 30

	#define COUPS_CONTROLE 4
	#define COUPS_VICTOIRE_GB 20
	#define NB_EVOL 1
	#define TOURS_GB 3
	#define TOURS_VIR 6

	#define VS 0
	#define IA 1

	//TABLEAU DES LISTES
	/*
		De façon à pouvoir utiliser les mêmes constantes,
		on surdimensionne le tableau
	*/
	#define DIM_TAB_LISTES 4

	//TABLEAU DES POINTS CLEFS
	#define DIM_TAB_POINTS_CLEFS 5

	//TYPE DE PION A PLACER
	#define CASE 0
	#define GLOBULE 1
	#define VIRUS 2
	#define GR 3

	//TYPE DE CASE
	#define NORMALE 0
	#define BLOQUEE 1
	#define CONTROLE 2
	#define ENTREE_VIRUS 3
	#define ENTREE_GR 4

	//VITESSES
	#define VITESSE_INI_VIRUS 4
	#define VITESSE_INI_GB 4
	#define VITESSE_INI_GR 4

	//TAILLE DU MASQUE
	#define VITESSE_MAX 4
	#define MASQUE_MAX (VITESSE_MAX * 2 + 1)


	struct coord
	{
		int x;
		int y;
	}; typedef struct coord coord;

	struct S_pion
	{
		int type_pion;
		int niveau;
		int vitesse;
		coord pos;
		struct S_pion* suiv;
	}; typedef struct S_pion S_pion;

	struct S_case
	{
		S_pion* pion;
		int type;
	}; typedef struct S_case S_case;

	struct S_masque
    {
        int occupe ; // 0 si libre , 1 si ami ou bloque , 2 si ennemi
        int poids ;
    }; typedef struct S_masque S_masque;
#endif
