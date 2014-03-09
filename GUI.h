#ifndef _GUI_H_
#define _GUI_H_

#include <SDL.h>
#include "Controls_GUI.h"

enum Interface
{
    TITRE = 0,
	MENU_PRINCIPALE = 1,
	COMMENT_JOUER = 2,
	EN_JEU = 3,
	FIN_JEU = 4,
	OPTIONS = 5,
	QUITTER = 6
};

// Classe qui gère les différentes interfaces
class GUI
{
private:
    static SDL_Surface* s_Ecran;
    static int s_BureauWidth;
    static int s_BureauHeight;
    static int s_EcranWidth;
    static int s_EcranHeight;
    static bool s_PleinEcran;
    static int s_NumStage;

    static void dessineHUD(SDL_Surface* iconeVaisseau, Text& score);
public:
    static Interface s_InterfaceActuelle;

    static int getWidth();
    static int getHeight();

    static SDL_Surface* getEcran();

    static void InitialiseEcran(int widthEcran, int heightEcran, bool pleinEcran);
    static bool EnJeu();
    static void FinJeu();
    static void MenuPrincipal();
    static void MenuOptions();
    static void Titre();
    static void CommentJouer();
    static void ChoixStage();

    static void dessineChargement(int ajout_val = 0, bool reinitialise = false, bool derniereVal = false);
};

#endif // _GUI_H_
