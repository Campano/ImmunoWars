#ifndef AFFICHAGE
	#define AFFICHAGE

	#include "structures.h"

	#ifdef MAC
		#define CLS() system("clear");

		#define CHAR_VIDE 32
		#define CHAR_GLOB 103
		#define CHAR_VIR 118
		#define CHAR_GR 114
		#define CHAR_BLOQ 88
		#define CHAR_CTRL 67
		#define CHAR_EVIR 86
		#define CHAR_EGR 82
	#endif

	#ifdef WINDOWS
		#define CLS() system("cls");

		#define CHAR_VIDE 176
		#define CHAR_GLOB 248
		#define CHAR_VIR 249
		#define CHAR_GR 114
		#define CHAR_BLOQ 178
		#define CHAR_CTRL 67
		#define CHAR_EVIR 68
		#define CHAR_EGR 69
	#endif

	#define NL() printf("\n");
	#define NLL() printf("\n\n");

	char int2char(int nb);
	void afficher_plateau(S_case plateau[TAILLE_PLAT][TAILLE_PLAT]);

	void afficher_joueur(const int joueur);
	void demander_coord(coord* case_a_jouer, int choix);

#endif
