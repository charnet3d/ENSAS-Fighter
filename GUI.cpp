#include "GUI.h"
#include <sstream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Timer.h"
#include "EntreeSortie.h"
#include "NFont.h"
#include "Controls_GUI.h"
#include <sprig.h>

using namespace std;

SDL_Surface* GUI::s_Ecran = NULL;
int GUI::s_BureauWidth;
int GUI::s_BureauHeight;
int GUI::s_EcranWidth;
int GUI::s_EcranHeight;
bool GUI::s_PleinEcran;
Interface GUI::s_InterfaceActuelle = TITRE;
int GUI::s_NumStage = 1;

int GUI::getWidth()
{
    return s_EcranWidth;
}
int GUI::getHeight()
{
        return s_EcranHeight;
}

SDL_Surface* GUI::getEcran()
{
    return s_Ecran;
}

void GUI::InitialiseEcran(int widthEcran, int heightEcran, bool pleinEcran)
{
    s_EcranWidth = widthEcran;
    s_EcranHeight = heightEcran;
    s_PleinEcran = pleinEcran;

    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    s_BureauWidth = info->current_w;
    s_BureauHeight = info->current_h;

    s_Ecran = SDL_SetVideoMode(s_EcranWidth, s_EcranHeight, 32, (s_PleinEcran?SDL_FULLSCREEN:0) | SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!s_Ecran)
        std::cerr << "Initialisation de la fenêtre échouée!";

    if (s_PleinEcran)
        SDL_ShowCursor(SDL_DISABLE);
    else
        SDL_ShowCursor(SDL_ENABLE);
}

void GUI::dessineHUD(SDL_Surface* iconeVaisseau, Text& score)
{
    // D'abord on dessine le nombre de vaisseau restants
    int tmpX;
    int widthZone = (s_EcranWidth / 3);
    // On calcule la distance séparant les icones des vaisseaux
    int separateur = (((widthZone - (60 * (s_EcranWidth / 1024.f))) - (5 * iconeVaisseau->w))) / 4;

    // dessine à l'ecran des icone du vaisseau
    VaisseauJoueur* vaisseau = VaisseauJoueur::getSingleton();
    tmpX = (int)(20 * s_EcranWidth / 1024.f);
    for (int i = 0; i < vaisseau->getVaisseauxRestants(); i++)
    {
        Entite::dessineSurface(tmpX, 0, iconeVaisseau, s_Ecran);
        tmpX += iconeVaisseau->w + separateur;
    }

    // Dessin de la barre de niveau de vie
    Uint32 couleur;
    if (vaisseau->getNiveauVie() >= 0.67 * VaisseauJoueur::s_NiveauVieMax)
        couleur = SDL_MapRGB(s_Ecran->format, 0, 185, 0);
    else if (vaisseau->getNiveauVie() < 0.67 * VaisseauJoueur::s_NiveauVieMax &&
             vaisseau->getNiveauVie() >= 0.34 * VaisseauJoueur::s_NiveauVieMax)
        couleur = SDL_MapRGB(s_Ecran->format, 255, 185, 0);
    else
        couleur = SDL_MapRGB(s_Ecran->format, 255, 0, 0);

    tmpX = widthZone;
    SPG_RectFilled(s_Ecran, tmpX, 0, tmpX + (widthZone * (vaisseau->getNiveauVie() / (float)VaisseauJoueur::s_NiveauVieMax)),
                    iconeVaisseau->h, couleur);

    SPG_Rect(s_Ecran, tmpX, 0, tmpX + widthZone, iconeVaisseau->h,
             SDL_MapRGB(s_Ecran->format, 255, 255, 255));

    tmpX += widthZone + (int)(20 * s_EcranWidth / 1024.f);

    stringstream ss;
    ss << "Score: " << Stage::getScore();
    score.setText(ss.str());

    score.setX(tmpX);
    score.setY((iconeVaisseau->h - score.getHeight()) / 2);
    score.dessineText();
}

void GUI::dessineChargement(int ajout_val, bool reinitialise, bool derniereVal)
{
    static SDL_Surface* arrierePlan;
    static NFont* policeChargement;
    static Text* txtChargement;
    static int max_val = 0;
    static int val = 0;
    if (reinitialise)
    {
        arrierePlan = Entite::chargeImg("Media\\ArrierePlanMenu.jpg", true);;
        policeChargement = new NFont(s_Ecran, Entite::chargeImg("Media\\PoliceMenu.png"));
        txtChargement = new Text(policeChargement, "Chargement...");
        max_val = 0;
        val = 0;
    }
    max_val += ajout_val;

    if(derniereVal)
        val++;

    // Initialisation du text
    txtChargement->setX((s_EcranWidth - txtChargement->getWidth()) / 2);
    txtChargement->setY((s_EcranHeight - (txtChargement->getHeight() + 30)) / 2);

    // Barre de chargement
    int tmpX = (s_EcranWidth - 300) / 2;
    int tmpY = txtChargement->getY() + txtChargement->getHeight() + 10;

    Entite::dessineSurface(0, 0, arrierePlan, s_Ecran);
    txtChargement->dessineText();

    SPG_RectFilled(s_Ecran, tmpX, tmpY, tmpX + (300 * (val / (float)max_val)),
                    tmpY + 20, SDL_MapRGB(s_Ecran->format, 0, 0, 185));

    SPG_Rect(s_Ecran, tmpX, tmpY, tmpX + 300, tmpY + 20,
             SDL_MapRGB(s_Ecran->format, 0, 0, 0));

    SDL_Flip(s_Ecran);

    if (derniereVal)
    {
        SDL_FreeSurface(arrierePlan);
        delete policeChargement;
        delete txtChargement;
    }

    val++;
}

void GUI::ChoixStage()
{
    // La fonction EnJeu() retourne true si le stage est complet
    bool stageTermine;
    do
    {
        stageTermine = GUI::EnJeu();
        s_NumStage++;
    } while (s_InterfaceActuelle != FIN_JEU && stageTermine);

    Stage::reinistaliseScore();
    s_NumStage = 1;

    if (stageTermine)
    {
        s_InterfaceActuelle = FIN_JEU;
        VaisseauJoueur::libere();
    }
}

bool GUI::EnJeu()
{
    const int IMAGES_PAR_SECONDE = 60;      // La limite d'images par seconde
    Timer fps;                              // Timer pour régulariser le nombre d'images par seconde (Frames Per Seconde - FPS)
    Timer compteurFps;                      // Timer pour compter le nombre d'images par seconde qui sera affiché par la suite
    int images = 0;                         // le nombre d'images dessinées à l'écran

    // Vérification si le stage existe, car sinon on est arrivé à la fin du jeu
    bool stageExist;
    EntreeSortie::chargeStage(s_NumStage, &stageExist);
    if (!stageExist)
    {
        s_InterfaceActuelle = FIN_JEU;
        return true;            // stageTermine = true
    }

    // chargement et redimensionnement de l'arrière plan
    dessineChargement(1, true, false);

    SDL_Surface* imgGameOver = Entite::chargeImg("Media\\Perdu.png", true, false);
    SDL_Surface* imgStageComplet = Entite::chargeImg("Media\\StageComplet.png", true, false);
    SDL_Surface* imgDetruisez = Entite::chargeImg("Media\\Detruisez.png", true, false);
    SDL_Surface* imgCadreDetruisez = Entite::chargeImg("Media\\CadreDetruisez.png", true, false);

    // Initialisation de la police d'écriture
    NFont* policeMessage = new NFont(s_Ecran, Entite::chargeImg("Media\\PoliceMessage.png"));

    stringstream ss;
    ss << "Stage " << s_NumStage;
    Text txtStage(policeMessage, ss.str().c_str());

    txtStage.setX((s_EcranWidth - txtStage.getWidth()) / 2);
    txtStage.setY(((s_EcranHeight - (txtStage.getHeight() + imgDetruisez->h)) / 2) - (int)(100 * (s_EcranHeight / 768.f)));

    Text txtMessage(policeMessage, "Votre vaisseau est detruit!");
    txtMessage.setX((s_EcranWidth - txtMessage.getWidth()) / 2);
    txtMessage.setY((s_EcranHeight - txtMessage.getHeight()) / 2);

    Stage* stageActuel = EntreeSortie::chargeStage(s_NumStage);

    dessineChargement(3);

    VaisseauJoueur* vaisseau = EntreeSortie::chargeJoueur();

    // On charge l'image de l'icone du vaisseau pour le nombre de vie restantes
    // et on la redimensionne selon l'ecran
    SDL_Surface* iconeVaisseau = Entite::chargeImg("Media\\IconeVaisseau.png", true);

    // Initialisation de la police d'écriture du score
    NFont* policeScore = new NFont(s_Ecran, Entite::chargeImg("Media\\PoliceScore.png"));

    Text txtScore(policeScore, "0");

    dessineChargement(0);

    // Initialisation de la police d'écriture du menu pause
    NFont* policeMenu = new NFont(s_Ecran, Entite::chargeImg("Media\\PoliceTouche.png"));
    NFont* policePause = new NFont(s_Ecran, Entite::chargeImg("Media\\PoliceMenu.png"));

    int arrowX = 0, arrowY = 0;
    SDL_Surface *arrow = Entite::chargeImg("Media\\ArrowPetit.png");

    // chargement et redimensionnement de l'arrière plan
    SDL_Surface* imgCadreMenuPause = Entite::chargeImg("Media\\CadreMenuPause.png");
    int cadreY = (s_EcranHeight - imgCadreMenuPause->h) / 2;
    int cadreX = (s_EcranWidth - imgCadreMenuPause->w) / 2;

    int selection = 1;

    // Initialisation des boutons
    Text txtPause(policePause, "Pause");
    Text btnMenuPrincipal(policeMenu, "Retour au menu principal");
    Text btnQuitter(policeMenu, "Quitter");

    txtPause.setX((s_EcranWidth - txtPause.getWidth()) / 2);
    txtPause.setY(cadreY + 20);

    btnMenuPrincipal.setX((s_EcranWidth - btnMenuPrincipal.getWidth()) / 2);
    btnMenuPrincipal.setY(txtPause.getY() + txtPause.getHeight() + 30);

    btnQuitter.setX((s_EcranWidth - btnQuitter.getWidth()) / 2);
    btnQuitter.setY(btnMenuPrincipal.getY() + btnMenuPrincipal.getHeight() + 30);

    dessineChargement(0, false, true);
    SDL_Delay(200);

    bool fin = false;
    bool leftKey = false;
    bool rightKey = false;
    bool spaceKey = false;
    bool stageComplet = false;
    bool jeuEnPause = false;
    SDL_Event event;

    bool afficheMort = false, afficheGameOver = false, afficheStageComplet = false,
        afficheDebutStage = true, affichePause = false;
    Timer timerFlash, timerMort, timerGameOver, timerStageComplet, timerDebutStage, timerFlashPause;

    // des variables d'aide pour l'écriture du texte sur l'écran
    stringstream ssFps("Images Par Seconde: ");

    // On démarrge le timer qui nous aide à calculer le nombre d'images par seconde
    compteurFps.demarre();

    // On demarre le timer pour afficher le message de début de stage
    timerDebutStage.demarre();
    timerFlash.demarre();

    while (!fin)
    {
        fps.demarre();

        // Gestion du dessin à l'écran
        // Arrière Plan
        stageActuel->dessineArrierePlan();

        // Vaisseau et ennemis (chacun avec ses missiles)
        Ennemi::dessineMissiles(jeuEnPause);
        stageActuel->getMatriceEnnemi()->dessineEnnemis(jeuEnPause);
        VaisseauJoueur::dessineMissiles(jeuEnPause);
        vaisseau->dessineEntite(jeuEnPause);

        dessineHUD(iconeVaisseau, txtScore);

        if (afficheMort)
            txtMessage.dessineText();
        if (afficheGameOver)
            Entite::dessineSurface((s_EcranWidth - imgGameOver->w) / 2,
                                    (s_EcranHeight - imgGameOver->h) / 2, imgGameOver, s_Ecran);

        if (afficheStageComplet)
            Entite::dessineSurface((s_EcranWidth - imgStageComplet->w) / 2,
                                    (s_EcranHeight - imgStageComplet->h) / 2, imgStageComplet, s_Ecran);

        // Le numero de stage s'affiche sans clignotement
        if (timerDebutStage.estDemarre())
        {
            Entite::dessineSurface((s_EcranWidth - imgCadreDetruisez->w) / 2,
                                    txtStage.getY() - (int)(30 * (s_EcranHeight / 768.f)),
                                    imgCadreDetruisez, s_Ecran);
            txtStage.dessineText();
        }

        // Le text "détruisez les tous" clignote
        if (afficheDebutStage)
            Entite::dessineSurface((s_EcranWidth - imgDetruisez->w) / 2,
                                    txtStage.getY() +  txtStage.getHeight() + (int)(20 * (s_EcranHeight / 768.f)),
                                    imgDetruisez, s_Ecran);

        // Tout ce qui change l'état du jeu ne doit être exécuté si
        // le jeu n'est pas en pause
        if (!jeuEnPause)
        {
            // Met à jour l'écran
            SDL_Flip(s_Ecran);

            // Gestion des mouvements
            vaisseau->bougeGauche(leftKey);
            vaisseau->bougeDroite(rightKey);

            if (!timerDebutStage.estDemarre())
            {
                vaisseau->attaque(spaceKey);
                stageActuel->getMatriceEnnemi()->bougeEnnemis();
                stageActuel->getMatriceEnnemi()->tirEnnemis();
            }

            // On affiche le message après la complétion de stage
            if (stageActuel->getMatriceEnnemi()->getTailleLignes() == 0)
            {
                if (!timerStageComplet.estDemarre())
                    timerStageComplet.demarre();

                if (!timerFlash.estDemarre())
                {
                    afficheStageComplet = true;
                    timerFlash.demarre();
                }
            }

            // On affiche le message après la mort
            if (!timerGameOver.estDemarre() && vaisseau->getNiveauVie() == 0)
            {
                if (!timerMort.estDemarre())
                    timerMort.demarre();
                if (!timerFlash.estDemarre())
                {
                    afficheMort = true;
                    timerFlash.demarre();
                }
            }

            // On flash le message de mort ou de stage complet
            if (timerFlash.getTemps() > 500)
            {
                if (timerStageComplet.estDemarre())
                {
                    afficheStageComplet = !afficheStageComplet;
                    timerFlash.demarre();
                }
                else if (timerDebutStage.estDemarre())
                {
                    afficheDebutStage = !afficheDebutStage;
                    timerFlash.demarre();
                }
                else
                {
                    afficheMort = !afficheMort;
                    timerFlash.demarre();
                }
            }

            // On affiche le message de début de stage
            if (timerDebutStage.getTemps() > 3000)
            {
                afficheDebutStage = false;
                timerFlash.arrete();
                timerDebutStage.arrete();
            }

            // On fait disparaitre le message de mort après apparaition du nouveau
            // Vaisseau, au cas ou il ne reste plus de vaisseau, on affiche GameOver
            if (timerMort.getTemps() > 1000)
            {
                if ((vaisseau->getVaisseauxRestants() > 0) ||
                (vaisseau->getVaisseauxRestants() == 0 && vaisseau->getNiveauVie() > 0))
                {
                    afficheMort = false;
                    timerFlash.arrete();
                    timerMort.arrete();
                }
                else if (timerMort.getTemps() > 2500 &&
                         vaisseau->getVaisseauxRestants() == 0 &&
                         vaisseau->getNiveauVie() == 0)
                {
                    afficheMort = false;
                    timerFlash.arrete();
                    timerMort.arrete();

                    afficheGameOver = true;

                    if (!timerGameOver.estDemarre())
                        timerGameOver.demarre();
                }
            }

            // On affiche le message perdu pendant un moment
            // avant de revenir au menu principal
            if (timerGameOver.getTemps() > 4000)
            {
                afficheGameOver = false;
                timerGameOver.arrete();

                // On réinitialise le score
                s_InterfaceActuelle = MENU_PRINCIPALE;
                fin = true;
            }

            // On affiche le message StageComplet pendant un moment (en flashant)
            // avant de passer au stage suivant
            if (timerStageComplet.getTemps() > 4000)
            {
                afficheStageComplet = false;
                timerFlash.arrete();
                timerStageComplet.arrete();
                stageComplet = true;
                fin = true;
            }

            // Gestion des Entrés (clavier/manette)
            SDL_PollEvent(&event);
            switch(event.type)
            {
            case SDL_QUIT:
                s_InterfaceActuelle = QUITTER;
                fin = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    leftKey = true;
                    break;
                case SDLK_RIGHT:
                    rightKey = true;
                    break;
                case SDLK_SPACE:
                    spaceKey = true;
                    break;
                case SDLK_ESCAPE:
                    leftKey = false;
                    rightKey = false;
                    spaceKey = false;
                    jeuEnPause = true;
                    timerFlash.pause();
                    timerMort.pause();
                    timerGameOver.pause();
                    timerStageComplet.pause();
                    timerDebutStage.pause();
                    timerFlashPause.demarre();
                    affichePause = true;
                    SDL_EnableKeyRepeat(10, 300);
                    break;
                default:
                    break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    leftKey = false;
                    break;
                case SDLK_RIGHT:
                    rightKey = false;
                    break;
                case SDLK_SPACE:
                    spaceKey = false;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            event.key.keysym.sym = SDLK_UNKNOWN;
        }
        else        // Le jeu est en pause
        {
            if (selection == 1)
            {
                arrowX = btnMenuPrincipal.getX() - arrow->w - 20;
                arrowY = btnMenuPrincipal.getY() - ((arrow->h - btnMenuPrincipal.getHeight()) / 2);
            }
            else if (selection == 2)
            {
                arrowX = btnQuitter.getX() - arrow->w - 20;
                arrowY = btnQuitter.getY() - ((arrow->h - btnQuitter.getHeight()) / 2);
            }

            // Gestion du dessin à l'écran
            Entite::dessineSurface(cadreX, cadreY, imgCadreMenuPause, s_Ecran);

            if (affichePause)
                txtPause.dessineText();
            btnMenuPrincipal.dessineText();
            btnQuitter.dessineText();

            Entite::dessineSurface(arrowX, arrowY, arrow, s_Ecran);

            // Met à jour l'écran
            SDL_Flip(s_Ecran);

            if (timerFlashPause.getTemps() > 500)
            {
                affichePause = !affichePause;
                timerFlashPause.demarre();
            }

            // Gestion des Entrés (clavier/manette)
            SDL_PollEvent(&event);
            switch(event.type)
            {
            case SDL_QUIT:
                s_InterfaceActuelle = QUITTER;
                fin = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    selection--;
                    if (selection == 0)
                        selection = 2;
                    event.key.keysym.sym = SDLK_UNKNOWN;
                    break;
                case SDLK_DOWN:
                    selection++;
                    if (selection == 3)
                        selection = 1;
                    event.key.keysym.sym = SDLK_UNKNOWN;
                    break;
                case SDLK_RETURN:
                    switch (selection)
                    {
                    case 1:
                        s_InterfaceActuelle = MENU_PRINCIPALE;
                        fin = true;
                        break;
                    case 2:
                        s_InterfaceActuelle = QUITTER;
                        fin = true;
                        break;
                    default:
                        break;
                    }
                    break;
                case SDLK_ESCAPE:
                    jeuEnPause = false;
                    timerFlash.unpause();
                    timerMort.unpause();
                    timerGameOver.unpause();
                    timerStageComplet.unpause();
                    timerDebutStage.unpause();
                    timerFlashPause.arrete();
                    SDL_EnableKeyRepeat(0, 0);
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            event.key.keysym.sym = SDLK_UNKNOWN;
        }

        // On teste si l'itération de la boucle (calcule et création de l'image à l'écran)
        // a pris moins que le temps reservé à une image par seconde (dans ce cas 1/FPS secondes ou 1000/FPS millisecondes)
        // FPS ici est le nombre maximal qu'on veux d'images par seconde
        if ((1000 / IMAGES_PAR_SECONDE) > fps.getTemps())
        {
            // On attend jusqu'à la fin du temps reservé à l'image actuelle
            SDL_Delay((1000 / IMAGES_PAR_SECONDE) - fps.getTemps());
        }

        images++;
    }

    delete policeMessage;
    delete policeScore;
    delete policeMenu;
    delete policePause;
    SDL_FreeSurface(imgGameOver);
    SDL_FreeSurface(imgStageComplet);
    SDL_FreeSurface(imgDetruisez);
    SDL_FreeSurface(imgCadreDetruisez);
    SDL_FreeSurface(iconeVaisseau);
    SDL_FreeSurface(imgCadreMenuPause);
    SDL_FreeSurface(arrow);

    if (s_InterfaceActuelle != EN_JEU)
        VaisseauJoueur::libere();
    VaisseauJoueur::viderListeMissiles();
    delete stageActuel;

    return stageComplet;
}

void GUI::FinJeu()
{
    const int IMAGES_PAR_SECONDE = 60;      // La limite d'images par seconde
    Timer fps;                              // Timer pour régulariser le nombre d'images par seconde (Frames Per Seconde - FPS)

    // Initialisation de la police d'écriture
    NFont* policeTouche = new NFont(s_Ecran, Entite::chargeImg("Media\\PoliceTouche.png"));

    Timer timerFlash;
    Timer timerAttente;
    timerAttente.demarre();
    bool affiche = false;

    // Initialisation des boutons
    Text txtTouche(policeTouche, "Appuyez sur une touche pour continuer");

    // chargement et redimensionnement de l'arrière plan
    SDL_Surface* titre = Entite::chargeImg("Media\\EcranTitre.jpg", true);

    txtTouche.setX((s_EcranWidth - txtTouche.getWidth()) / 2);
    txtTouche.setY((int)(450 * s_EcranHeight / 768.f));

    bool fin = false;
    SDL_Event event;

    while (!fin)
    {
        fps.demarre();

        // Gestion du dessin à l'écran
        Entite::dessineSurface(0, 0, titre, s_Ecran);

        if (affiche)
            txtTouche.dessineText();

        if (timerAttente.getTemps() > 2000)
        {
            timerAttente.arrete();
            timerFlash.demarre();
            affiche = true;
        }

        if (timerFlash.getTemps() > 500)
        {
            affiche = !affiche;
            timerFlash.demarre();
        }

        // Met à jour l'écran
        SDL_Flip(s_Ecran);

        // Gestion des Entrés (clavier/manette)
        SDL_PollEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            s_InterfaceActuelle = QUITTER;
            fin = true;
            break;
        case SDL_KEYDOWN:
            if (!timerAttente.estDemarre())
            {
                s_InterfaceActuelle = MENU_PRINCIPALE;
                fin = true;
            }
            break;
        default:
            break;
        }
        event.key.keysym.sym = SDLK_UNKNOWN;

        // On teste si l'itération de la boucle (calcule et création de l'image à l'écran)
        // a pris moins que le temps reservé à une image par seconde (dans ce cas 1/FPS secondes ou 1000/FPS millisecondes)
        // FPS ici est le nombre maximal qu'on veux d'images par seconde
        if ((1000 / IMAGES_PAR_SECONDE) > fps.getTemps())
        {
            // On attend jusqu'à la fin du temps reservé à l'image actuelle
            SDL_Delay((1000 / IMAGES_PAR_SECONDE) - fps.getTemps());
        }
    }

    SDL_FreeSurface(titre);
    delete policeTouche;
}

void GUI::Titre()
{
    const int IMAGES_PAR_SECONDE = 60;      // La limite d'images par seconde
    Timer fps;                              // Timer pour régulariser le nombre d'images par seconde (Frames Per Seconde - FPS)

    // Initialisation de la police d'écriture
    NFont* policeTouche = new NFont(s_Ecran, Entite::chargeImg("Media\\PoliceTouche.png"));

    Timer timerFlash;
    timerFlash.demarre();
    bool affiche = false;

    // Initialisation des boutons
    Text txtTouche(policeTouche, "Appuyez sur une touche pour continuer");
    txtTouche.setX((s_EcranWidth - txtTouche.getWidth()) / 2);
    txtTouche.setY((int)(450 * s_EcranHeight / 768.f));

    // chargement et redimensionnement de l'arrière plan
    SDL_Surface* titre = Entite::chargeImg("Media\\EcranTitre.jpg", true);


    bool fin = false;
    SDL_Event event;

    while (!fin)
    {
        fps.demarre();

        // Gestion du dessin à l'écran
        Entite::dessineSurface(0, 0, titre, s_Ecran);

        if (affiche)
            txtTouche.dessineText();

        if (timerFlash.getTemps() > 500)
        {
            affiche = !affiche;
            timerFlash.demarre();
        }

        // Met à jour l'écran
        SDL_Flip(s_Ecran);

        // Gestion des Entrés (clavier/manette)
        SDL_PollEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            s_InterfaceActuelle = QUITTER;
            fin = true;
            break;
        case SDL_KEYDOWN:
            s_InterfaceActuelle = MENU_PRINCIPALE;
            fin = true;
            break;
        default:
            break;
        }
        event.key.keysym.sym = SDLK_UNKNOWN;

        // On teste si l'itération de la boucle (calcule et création de l'image à l'écran)
        // a pris moins que le temps reservé à une image par seconde (dans ce cas 1/FPS secondes ou 1000/FPS millisecondes)
        // FPS ici est le nombre maximal qu'on veux d'images par seconde
        if ((1000 / IMAGES_PAR_SECONDE) > fps.getTemps())
        {
            // On attend jusqu'à la fin du temps reservé à l'image actuelle
            SDL_Delay((1000 / IMAGES_PAR_SECONDE) - fps.getTemps());
        }
    }

    SDL_FreeSurface(titre);
    delete policeTouche;
}

void GUI::CommentJouer()
{
    const int IMAGES_PAR_SECONDE = 60;      // La limite d'images par seconde
    Timer fps;                              // Timer pour régulariser le nombre d'images par seconde (Frames Per Seconde - FPS)

    // Initialisation de la police d'écriture
    NFont* policeTouche = new NFont(s_Ecran, Entite::chargeImg("Media\\PoliceTouche.png"));

    Timer timerFlash;
    timerFlash.demarre();
    bool affiche = false;

    // Initialisation des boutons
    Text txtTouche(policeTouche, "Appuyez sur une touche pour revenir");
    txtTouche.setX((s_EcranWidth - txtTouche.getWidth()) / 2);
    txtTouche.setY(s_EcranHeight - (int)(100 * s_EcranHeight / 768.f));

    // chargement et redimensionnement de l'arrière plan
    SDL_Surface* titre = Entite::chargeImg("Media\\CommentJouer.jpg", true);


    bool fin = false;
    SDL_Event event;

    while (!fin)
    {
        fps.demarre();

        // Gestion du dessin à l'écran
        Entite::dessineSurface(0, 0, titre, s_Ecran);

        if (affiche)
            txtTouche.dessineText();

        if (timerFlash.getTemps() > 500)
        {
            affiche = !affiche;
            timerFlash.demarre();
        }

        // Met à jour l'écran
        SDL_Flip(s_Ecran);

        // Gestion des Entrés (clavier/manette)
        SDL_PollEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            s_InterfaceActuelle = QUITTER;
            fin = true;
            break;
        case SDL_KEYDOWN:
            s_InterfaceActuelle = MENU_PRINCIPALE;
            fin = true;
            break;
        default:
            break;
        }
        event.key.keysym.sym = SDLK_UNKNOWN;

        // On teste si l'itération de la boucle (calcule et création de l'image à l'écran)
        // a pris moins que le temps reservé à une image par seconde (dans ce cas 1/FPS secondes ou 1000/FPS millisecondes)
        // FPS ici est le nombre maximal qu'on veux d'images par seconde
        if ((1000 / IMAGES_PAR_SECONDE) > fps.getTemps())
        {
            // On attend jusqu'à la fin du temps reservé à l'image actuelle
            SDL_Delay((1000 / IMAGES_PAR_SECONDE) - fps.getTemps());
        }
    }

    SDL_FreeSurface(titre);
    delete policeTouche;
}

void GUI::MenuPrincipal()
{
    const int IMAGES_PAR_SECONDE = 60;      // La limite d'images par seconde
    Timer fps;                              // Timer pour régulariser le nombre d'images par seconde (Frames Per Seconde - FPS)

    // Initialisation de la police d'écriture
    NFont* policeMenu = new NFont(s_Ecran, Entite::chargeImg("Media\\PoliceMenu.png"));

    int arrowX = 0, arrowY = 0;
    SDL_Surface *arrow = Entite::chargeImg("Media\\Arrow.png");

    // chargement et redimensionnement de l'arrière plan
    SDL_Surface* arrierePlan = Entite::chargeImg("Media\\ArrierePlanMenu.jpg", true);

    int selection = 1;

    // Initialisation des boutons
    Text btnNouvellePartie(policeMenu, "Nouvelle Partie");
    Text btnCommentJouer(policeMenu, "Comment Jouer ?");
    Text btnOptions(policeMenu, "Options");
    Text btnQuitter(policeMenu, "Quitter");

    btnNouvellePartie.setX((s_EcranWidth - btnNouvellePartie.getWidth()) / 2);
    btnNouvellePartie.setY((s_EcranHeight - (btnNouvellePartie.getHeight() +
                                             btnCommentJouer.getHeight() +
                                             btnOptions.getHeight() +
                                             btnQuitter.getHeight() + 60)) / 2);

    btnCommentJouer.setX((s_EcranWidth - btnCommentJouer.getWidth()) / 2);
    btnCommentJouer.setY(btnNouvellePartie.getY() + btnNouvellePartie.getHeight() + 20);

    btnOptions.setX((s_EcranWidth - btnOptions.getWidth()) / 2);
    btnOptions.setY(btnCommentJouer.getY() + btnCommentJouer.getHeight() + 20);

    btnQuitter.setX((s_EcranWidth - btnQuitter.getWidth()) / 2);
    btnQuitter.setY(btnOptions.getY() + btnOptions.getHeight() + 20);

    bool fin = false;
    SDL_Event event;

    SDL_EnableKeyRepeat(10, 300);

    while (!fin)
    {
        fps.demarre();

        if (selection == 1)
        {
            arrowX = btnNouvellePartie.getX() - arrow->w - 10;
            arrowY = btnNouvellePartie.getY() - ((arrow->h - btnNouvellePartie.getHeight()) / 2);
        }
        else if (selection == 2)
        {
            arrowX = btnCommentJouer.getX() - arrow->w - 10;
            arrowY = btnCommentJouer.getY() - ((arrow->h - btnCommentJouer.getHeight()) / 2);
        }
        else if (selection == 3)
        {
            arrowX = btnOptions.getX() - arrow->w - 10;
            arrowY = btnOptions.getY() - ((arrow->h - btnOptions.getHeight()) / 2);
        }
        else if (selection == 4)
        {
            arrowX = btnQuitter.getX() - arrow->w - 10;
            arrowY = btnQuitter.getY() - ((arrow->h - btnQuitter.getHeight()) / 2);
        }

        // Gestion du dessin à l'écran
        Entite::dessineSurface(0, 0, arrierePlan, s_Ecran);

        btnNouvellePartie.dessineText();
        btnCommentJouer.dessineText();
        btnOptions.dessineText();
        btnQuitter.dessineText();

        Entite::dessineSurface(arrowX, arrowY, arrow, s_Ecran);

        // Met à jour l'écran
        SDL_Flip(s_Ecran);

        // Gestion des Entrés (clavier/manette)
        SDL_PollEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            s_InterfaceActuelle = QUITTER;
            fin = true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                selection--;
                if (selection == 0)
                    selection = 4;
                event.key.keysym.sym = SDLK_UNKNOWN;
                break;
            case SDLK_DOWN:
                selection++;
                if (selection == 5)
                    selection = 1;
                event.key.keysym.sym = SDLK_UNKNOWN;
                break;
            case SDLK_RETURN:
                switch (selection)
                {
                case 1:
                    s_InterfaceActuelle = EN_JEU;
                    fin = true;
                    break;
                case 2:
                    s_InterfaceActuelle = COMMENT_JOUER;
                    fin = true;
                    break;
                case 3:
                    s_InterfaceActuelle = OPTIONS;
                    fin = true;
                    break;
                case 4:
                    s_InterfaceActuelle = QUITTER;
                    fin = true;
                    break;
                default:
                    break;
                }
                break;
            case SDLK_ESCAPE:
                s_InterfaceActuelle = QUITTER;
                fin = true;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        event.key.keysym.sym = SDLK_UNKNOWN;

        // On teste si l'itération de la boucle (calcule et création de l'image à l'écran)
        // a pris moins que le temps reservé à une image par seconde (dans ce cas 1/FPS secondes ou 1000/FPS millisecondes)
        // FPS ici est le nombre maximal qu'on veux d'images par seconde
        if ((1000 / IMAGES_PAR_SECONDE) > fps.getTemps())
        {
            // On attend jusqu'à la fin du temps reservé à l'image actuelle
            SDL_Delay((1000 / IMAGES_PAR_SECONDE) - fps.getTemps());
        }
    }
    SDL_FreeSurface(arrierePlan);
    SDL_FreeSurface(arrow);
    delete policeMenu;
}

void GUI::MenuOptions()
{
    const int IMAGES_PAR_SECONDE = 60;      // La limite d'images par seconde
    Timer fps;                              // Timer pour régulariser le nombre d'images par seconde (Frames Per Seconde - FPS)

    // Initialisation de la police d'écriture
    NFont* policeMenu = new NFont(s_Ecran, Entite::chargeImg("Media\\PoliceMenu.png"));

    int arrowX = 0, arrowY = 0;
    SDL_Surface *arrow = Entite::chargeImg("Media\\Arrow.png");
    SDL_Surface *arrowSelectionDroit = Entite::chargeImg("Media\\ArrowSelectionDroit.png");
    SDL_Surface *arrowSelectionGauche = Entite::chargeImg("Media\\ArrowSelectionGauche.png");
    SDL_Surface *Coche = Entite::chargeImg("Media\\Coche.png");

    // chargement et redimensionnement de l'arrière plan
    SDL_Surface* arrierePlan = Entite::chargeImg("Media\\ArrierePlanMenu.jpg", true);

    // Initialisation de la liste des résolutions possibles
    vector<int> listeWidth;
    vector<int> listeHeight;

    SDL_Rect** modes;

    modes = SDL_ListModes(NULL, SDL_FULLSCREEN | SDL_HWSURFACE | SDL_DOUBLEBUF);

    // On test les 3 résolution les plus supportés et on les ajoute à la liste des options
    for (int i = 0; modes[i]; i++)
    {
        if (modes[i]->w == 640 && modes[i]->h == 480)
        {
            listeWidth.push_back(modes[i]->w);
            listeHeight.push_back(modes[i]->h);
        }
    }
    for (int i = 0; modes[i]; i++)
    {
        if (modes[i]->w == 800 && modes[i]->h == 600)
        {
            listeWidth.push_back(modes[i]->w);
            listeHeight.push_back(modes[i]->h);
        }
    }
    for (int i = 0; modes[i]; i++)
    {
        if (modes[i]->w == 1024 && modes[i]->h == 768)
        {
            listeWidth.push_back(modes[i]->w);
            listeHeight.push_back(modes[i]->h);
        }
    }
    // On ajout la résolution du bureau en plus des 3 résolutions en haut
    listeWidth.push_back(s_BureauWidth);
    listeHeight.push_back(s_BureauHeight);

    int selection = 1;
    int resolutionActuelle = 0;

    // On cherche l'indice de la résolution actuelle dans la liste des résolutions
    for (unsigned int i = 0; listeWidth.size(); i++)
    {
        if (listeWidth[i] == s_EcranWidth &&
            listeHeight[i] == s_EcranHeight)
        {
            resolutionActuelle = i;
            break;
        }
    }

    // Initialisation des boutons
    Text btnResolutionEcran(policeMenu, "Resolution d'ecran : ");
    Text btnChoixResolution(policeMenu, "1024 x 768");
    Text btnPleinEcran(policeMenu, "Plein ecran : ");
    Text btnSauvegarder(policeMenu, "Sauvegarder");
    Text btnRetour(policeMenu, "Retour");

    btnResolutionEcran.setX((s_EcranWidth - btnResolutionEcran.getWidth()) / 2);
    btnResolutionEcran.setY((s_EcranHeight - (btnResolutionEcran.getHeight() +
                                              btnChoixResolution.getHeight() +
                                              btnPleinEcran.getHeight() +
                                              btnSauvegarder.getHeight() +
                                              btnRetour.getHeight() + 85)) / 2);

    btnChoixResolution.setX((s_EcranWidth - btnChoixResolution.getWidth()) / 2);
    btnChoixResolution.setY(btnResolutionEcran.getY() + btnResolutionEcran.getHeight() + 5);

    btnPleinEcran.setX((s_EcranWidth - btnPleinEcran.getWidth()) / 2);
    btnPleinEcran.setY(btnChoixResolution.getY() + btnChoixResolution.getHeight() + 20);

    btnSauvegarder.setX((s_EcranWidth - btnSauvegarder.getWidth()) / 2);
    btnSauvegarder.setY(btnPleinEcran.getY() + btnPleinEcran.getHeight() + 40);

    btnRetour.setX((s_EcranWidth - btnRetour.getWidth()) / 2);
    btnRetour.setY(btnSauvegarder.getY() + btnSauvegarder.getHeight() + 20);

    // Initialisation de la police d'écriture du menu pause
    NFont* policeTextInfo = new NFont(s_Ecran, Entite::chargeImg("Media\\PoliceTouche.png"));

    SDL_Surface *arrowPetit = Entite::chargeImg("Media\\ArrowPetit.png");

    // chargement et redimensionnement de l'arrière plan
    SDL_Surface* imgCadreMenuPause = Entite::chargeImg("Media\\CadreMenuPause.png");
    int cadreY = (s_EcranHeight - imgCadreMenuPause->h) / 2;
    int cadreX = (s_EcranWidth - imgCadreMenuPause->w) / 2;

    // Initialisation des boutons
    Text txtInfo(policeMenu, "Information");
    Text txtTextInfo(policeTextInfo, "Vous devez redemarrer le jeu\npour que le changement\nde resolution prenne effet.");
    Text btnOK(policeMenu, "OK");

    txtInfo.setX((s_EcranWidth - txtInfo.getWidth()) / 2);
    txtInfo.setY(cadreY + 20);

    txtTextInfo.setX((s_EcranWidth - txtTextInfo.getWidth()) / 2);
    txtTextInfo.setY(txtInfo.getY() + txtInfo.getHeight() + 20);

    btnOK.setX((s_EcranWidth - btnOK.getWidth()) / 2);
    btnOK.setY(txtTextInfo.getY() + txtTextInfo.getHeight() + 25);

    int tmpX, tmpY;
    bool changeResolution = false;
    bool afficheCadreInfo = false, afficheInfo = false;
    //bool resolutionChange = false, pleinEcranChange = false;
    bool pleinEcran = s_PleinEcran;
    Timer timerFlash;
    bool fin = false;
    SDL_Event event;
    stringstream ss;

    SDL_EnableKeyRepeat(10, 300);

    while (!fin)
    {
        fps.demarre();

        if (selection == 1)
        {
            arrowX = btnResolutionEcran.getX() - arrow->w - 10;
            arrowY = btnResolutionEcran.getY() - ((arrow->h - btnResolutionEcran.getHeight()) / 2);
        }
        else if (selection == 2)
        {
            arrowX = btnPleinEcran.getX() - arrow->w - 10;
            arrowY = btnPleinEcran.getY() - ((arrow->h - btnPleinEcran.getHeight()) / 2);
        }
        else if (selection == 3)
        {
            arrowX = btnSauvegarder.getX() - arrow->w - 10;
            arrowY = btnSauvegarder.getY() - ((arrow->h - btnSauvegarder.getHeight()) / 2);
        }
        else if (selection == 4)
        {
            arrowX = btnRetour.getX() - arrow->w - 10;
            arrowY = btnRetour.getY() - ((arrow->h - btnRetour.getHeight()) / 2);
        }

        ss.str("");
        ss << listeWidth[resolutionActuelle] << " x " << listeHeight[resolutionActuelle];
        btnChoixResolution.setText(ss.str());

        // Gestion du dessin à l'écran
        Entite::dessineSurface(0, 0, arrierePlan, s_Ecran);

        if (changeResolution)
        {
            Entite::dessineSurface(btnChoixResolution.getX() - arrowSelectionGauche->w - 10,
                               btnChoixResolution.getY() - ((arrowSelectionGauche->h - btnChoixResolution.getHeight()) / 2),
                               arrowSelectionGauche, s_Ecran);

            Entite::dessineSurface(btnChoixResolution.getX() + btnChoixResolution.getWidth() + 10,
                                   btnChoixResolution.getY() - ((arrowSelectionDroit->h - btnChoixResolution.getHeight()) / 2),
                                   arrowSelectionDroit, s_Ecran);
        }

        btnResolutionEcran.dessineText();
        btnChoixResolution.dessineText();
        btnPleinEcran.dessineText();

        tmpX = btnPleinEcran.getX() + btnPleinEcran.getWidth() + 10;
        tmpY = btnPleinEcran.getY() - ((Coche->h * 0.8 - btnPleinEcran.getHeight()) / 2);
        SPG_Rect(s_Ecran, tmpX, tmpY, tmpX + Coche->w * 0.8, tmpY + Coche->h * 0.8,
                 SDL_MapRGB(s_Ecran->format, 0, 0, 0));

        if (pleinEcran)
            Entite::dessineSurface(tmpX - ((Coche->w - Coche->w * 0.8) / 2),
                                   tmpY - ((Coche->h - Coche->h * 0.8) / 2),
                                   Coche, s_Ecran);

        btnSauvegarder.dessineText();
        btnRetour.dessineText();

        Entite::dessineSurface(arrowX, arrowY, arrow, s_Ecran);


        if (afficheCadreInfo)
        {
            Entite::dessineSurface(cadreX, cadreY, imgCadreMenuPause, s_Ecran);

            if (afficheInfo)
                txtInfo.dessineText();
            txtTextInfo.dessineText();
            btnOK.dessineText();

            Entite::dessineSurface(btnOK.getX() - arrowPetit->w - 20,
                                   btnOK.getY() - ((arrowPetit->h - btnOK.getHeight()) / 2),
                                   arrowPetit, s_Ecran);

            if (timerFlash.getTemps() > 500)
            {
                afficheInfo = !afficheInfo;
                timerFlash.demarre();
            }
        }

        // Met à jour l'écran
        SDL_Flip(s_Ecran);

        // Gestion des Entrés (clavier/manette)
        SDL_PollEvent(&event);
        if (!afficheCadreInfo)
        {
            switch(event.type)
            {
            case SDL_QUIT:
                s_InterfaceActuelle = QUITTER;
                fin = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    if (!changeResolution)
                    {
                        selection--;
                        if (selection == 0)
                            selection = 4;
                        event.key.keysym.sym = SDLK_UNKNOWN;
                    }
                    break;
                case SDLK_DOWN:
                    if (!changeResolution)
                    {
                        selection++;
                        if (selection == 5)
                            selection = 1;
                        event.key.keysym.sym = SDLK_UNKNOWN;
                    }
                    break;
                case SDLK_LEFT:
                    if (changeResolution)
                    {
                        resolutionActuelle--;
                        if (resolutionActuelle == -1)
                            resolutionActuelle = listeWidth.size() - 1;
                        event.key.keysym.sym = SDLK_UNKNOWN;
                    }
                    break;
                case SDLK_RIGHT:
                    if (changeResolution)
                    {
                        resolutionActuelle++;
                        if (resolutionActuelle == (int)listeWidth.size())
                            resolutionActuelle = 0;
                        event.key.keysym.sym = SDLK_UNKNOWN;
                    }
                    break;
                case SDLK_RETURN:
                    switch (selection)
                    {
                    case 1:
                        changeResolution = !changeResolution;
                        if (!changeResolution &&
                           (s_EcranWidth != listeWidth[resolutionActuelle] ||
                            s_EcranHeight != listeHeight[resolutionActuelle]))
                        {
                            timerFlash.demarre();
                            afficheCadreInfo = true;
                            afficheInfo = true;
                        }

                        break;
                    case 2:
                        pleinEcran = !pleinEcran;
                        break;
                    case 3:
                        // On applique le paramètre plein écran
                        if (pleinEcran != s_PleinEcran)
                        {
                            SDL_FreeSurface(s_Ecran);
                            s_Ecran = SDL_SetVideoMode(s_EcranWidth, s_EcranHeight, 32, (pleinEcran?SDL_FULLSCREEN:0) | SDL_HWSURFACE | SDL_DOUBLEBUF);
                            if (!s_Ecran)
                                cerr << "Initialisation de la fenêtre échouée!";
                            s_PleinEcran = pleinEcran;

                            if (s_PleinEcran)
                                SDL_ShowCursor(SDL_DISABLE);
                            else
                                SDL_ShowCursor(SDL_ENABLE);
                        }
                        // On enregistre les options changés
                        EntreeSortie::EnregistreOptions(listeWidth[resolutionActuelle], listeHeight[resolutionActuelle], s_PleinEcran);
                        s_InterfaceActuelle = MENU_PRINCIPALE;
                        fin = true;
                        break;
                    case 4:
                        s_InterfaceActuelle = MENU_PRINCIPALE;
                        fin = true;
                        break;
                    default:
                        break;
                    }
                    break;
                case SDLK_ESCAPE:
                    s_InterfaceActuelle = MENU_PRINCIPALE;
                    fin = true;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            event.key.keysym.sym = SDLK_UNKNOWN;
        }
        else
        {
            switch(event.type)
            {
            case SDL_QUIT:
                s_InterfaceActuelle = QUITTER;
                fin = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_RETURN:
                    timerFlash.arrete();
                    afficheCadreInfo = false;
                    afficheInfo = false;
                    break;
                case SDLK_ESCAPE:
                    timerFlash.arrete();
                    afficheCadreInfo = false;
                    afficheInfo = false;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            event.key.keysym.sym = SDLK_UNKNOWN;
        }
        // On teste si l'itération de la boucle (calcule et création de l'image à l'écran)
        // a pris moins que le temps reservé à une image par seconde (dans ce cas 1/FPS secondes ou 1000/FPS millisecondes)
        // FPS ici est le nombre maximal qu'on veux d'images par seconde
        if ((1000 / IMAGES_PAR_SECONDE) > fps.getTemps())
        {
            // On attend jusqu'à la fin du temps reservé à l'image actuelle
            SDL_Delay((1000 / IMAGES_PAR_SECONDE) - fps.getTemps());
        }
    }
    SDL_FreeSurface(arrow);
    SDL_FreeSurface(arrowSelectionDroit);
    SDL_FreeSurface(arrowSelectionGauche);
    SDL_FreeSurface(Coche);
    SDL_FreeSurface(arrierePlan);
    SDL_FreeSurface(arrowPetit);
    SDL_FreeSurface(imgCadreMenuPause);
    delete policeMenu;
    delete policeTextInfo;
}
