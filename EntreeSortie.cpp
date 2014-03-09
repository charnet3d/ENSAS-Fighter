#include "EntreeSortie.h"
#include <fstream>
#include <sstream>
#include <SDL_ttf.h>
#include "GUI.h"

using namespace std;

bool EntreeSortie::initialiseSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        cerr << "Initialisation de SDL échouée!";
        return EXIT_FAILURE;
    }

    // chargement de la resolution du fichier de configuration
    ifstream fichierOptions("Data\\options.cfg");
    int widthEcran = 0, heightEcran = 0;
    bool pleinEcran;
    fichierOptions >> widthEcran >> heightEcran >> pleinEcran;
    fichierOptions.close();

    if (widthEcran == 0 && heightEcran == 0)
    {
        const SDL_VideoInfo* info = SDL_GetVideoInfo();
        widthEcran = info->current_w;
        heightEcran = info->current_h;
        pleinEcran = true;
    }

    GUI::InitialiseEcran(widthEcran, heightEcran, pleinEcran);

    SDL_Surface* icon = Entite::chargeImg("Media\\mainPetite.bmp");
    SDL_SetColorKey(icon, SDL_SRCCOLORKEY, SDL_MapRGB(icon->format, 255, 0, 255));
    SDL_WM_SetIcon(icon, NULL);
    SDL_WM_SetCaption("ENSAS Fighter", NULL);
    SDL_EnableUNICODE(true);

    TTF_Init();
    return EXIT_SUCCESS;
}

void EntreeSortie::EnregistreOptions(int widthEcran, int heightEcran, bool pleinEcran)
{
    ofstream fichierOptions("Data\\options.cfg");
    fichierOptions << widthEcran << " " << heightEcran << " " << pleinEcran;
    fichierOptions.close();
}

Stage* EntreeSortie::chargeStage(int numStage, bool* stageExist)
{
    // On crée le nom du fichier de stage à partir du numéro de stage
    stringstream ss;
    ss << "Data\\Stage" << numStage << ".stg";

    // On ouvre le fichier de stage
    ifstream fichierStage(ss.str().c_str());

    if (stageExist)
    {
        if (fichierStage)
            *stageExist = true;
        else
            *stageExist = false;
        return NULL;
    }
    else
    {
        if (fichierStage)
        {
            int nbrTirSimultane;
            int intervalTirEnnemi;
            string ennemis, fichierArrierePlan, tmp;

            fichierStage >> intervalTirEnnemi;
            fichierStage >> nbrTirSimultane;
            getline(fichierStage, tmp);     // Pour enlever le retour à la ligne qui reste
            getline(fichierStage, fichierArrierePlan);

            while (!fichierStage.eof())
            {
                getline(fichierStage, tmp);
                ennemis += tmp + (!fichierStage.eof()?"\n":"");
            }
            fichierStage.close();

            return new Stage(ennemis, intervalTirEnnemi, nbrTirSimultane, fichierArrierePlan);
        }
        else
            return NULL;
    }
}

VaisseauJoueur* EntreeSortie::chargeJoueur()
{
    VaisseauJoueur* vaisseau = VaisseauJoueur::getSingleton("Media\\VaisseauJoueur.png", 0, 0, 94, 100);
    vaisseau->setPos((GUI::getWidth() - vaisseau->getRect().w) / 2, GUI::getHeight() - vaisseau->getRect().h);
    return vaisseau;
}

void EntreeSortie::libere()
{
    VaisseauJoueur::libere();

    TTF_Quit();
    SDL_Quit();
}


