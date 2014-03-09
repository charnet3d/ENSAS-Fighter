#include "VaisseauJoueur.h"
#include "GUI.h"

using namespace std;

VaisseauJoueur* VaisseauJoueur::s_Instance = NULL;
bool VaisseauJoueur::s_InstanceCree = false;
std::list<MissileJoueur> VaisseauJoueur::s_ListeMissiles;
int VaisseauJoueur::s_NiveauVieMax = 100;

void VaisseauJoueur::chargerImages()
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 4; j++)
            ajouterImage(m_PartieImageWidth * j + j, m_PartieImageHeight * i + i, m_PartieImageWidth, m_PartieImageHeight);
    ajouterImage(0, m_PartieImageHeight * 5 + 5, m_PartieImageWidth, m_PartieImageHeight);

    int partieImageWidth, partieImageHeight;

    partieImageWidth = (m_SurfaceSourceExplosion->w - 3) / 4;
    partieImageHeight = (m_SurfaceSourceExplosion->h - 1) / 2;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 4; j++)
            ajouterImageExplosion(partieImageWidth * j + j, partieImageHeight * i + i, partieImageWidth, partieImageHeight);

    m_ModeleMissile = new MissileJoueur("Media\\MissileJoueur.png", 0, 0, 7, 29);
}

VaisseauJoueur::VaisseauJoueur(const string& fichier, int x, int y, int partieImageWidth, int partieImageHeight)
: Entite(fichier, "Media\\ExplosionMoyenne.png", x, y, true), m_TimerAcceleration(25),
    m_TimerGlissage(45), m_TimerAttaque(500),
    m_TimerAnimation(120), m_TimerHit()
{
    m_PartieImageWidth = partieImageWidth;
    m_PartieImageHeight = partieImageHeight;

    chargerImages();

    // On redimensionne les images selon la résolution de l'écran actuelle
    redimensionne();

    setEtatAnimeActuel(0);

    m_Vitesse = 0;
    m_NiveauVie = s_NiveauVieMax;
    m_VaisseauxRestants = 5;
}

/*
La fonction statique getSingleton

Utilisation:
Pour avoir l'instance de l'objet, on doit d'abord l'appeler avec les paramètres pour initialiser l'objet
Après la première création, les paramètres ne serviront plus à rien, alors on pourra directement l'appeler sans paramètres
Si on essai de l'appeler sans paramètres alors que c'est la première fois (on doit initialiser les attributs) dans ce cas on renvoie NULL

Paramètres:
fichier: nom du fichier image pour la SDL_Surface
x, y: les coordonnés de départ

Valeur de retour: Une réference à notre unique objet
*/
VaisseauJoueur* VaisseauJoueur::getSingleton(const string& fichier, int x, int y, int partieImageWidth, int partieImageHeight)
{
    if (!s_InstanceCree)
    {
        if (fichier == "")
        {
            // Erreur, l'instance n'est pas encore crée et on essai de d'appeler getSingleton() sans paramètres
            // Alors on renvoie NULL
            return NULL;
        }
        else
        {
            // On crée l'objet en utilisant les paramètres spécifiés
            s_Instance = new VaisseauJoueur(fichier, x, y, partieImageWidth, partieImageHeight);
            s_InstanceCree = true;
            return s_Instance;
        }
    }
    else
    {
        // Dans le cas où on donne des paramètres on les ignore tout simplement car l'objet est déjà crée
        // On renvoie l'unique instance de l'objet
        return s_Instance;
    }
}

void VaisseauJoueur::libere()
{
    if (s_InstanceCree)
        delete s_Instance;
}

void VaisseauJoueur::bougeGauche(bool active)
{
    if (getNiveauVie())
    {
        if (active)
        {
            m_Rect.x += m_Vitesse;

            if (m_Rect.x <= 0)
            {
                m_Rect.x = 0;
                m_Vitesse = -m_Vitesse / 2;
            }
            else
            {
                if (m_TimerAcceleration.verifie())
                {
                    if (m_Vitesse > -12)
                        m_Vitesse--;
                }
            }
        }
        else
        {
            if (m_Vitesse != 0)
            {
                m_Rect.x += m_Vitesse;

                if (m_Rect.x <= 0)
                {
                    m_Rect.x = 0;
                    m_Vitesse = -m_Vitesse / 2;
                }

                if (m_TimerGlissage.verifie())
                    if (m_Vitesse != 0)
                        m_Vitesse += (m_Vitesse > 0)?-1:((m_Vitesse < 0)?1:0);
            }
        }
    }
    else
        m_Vitesse = 0;
}

void VaisseauJoueur::bougeDroite(bool active)
{
    if (getNiveauVie())
    {
        if (active)
        {
            m_Rect.x += m_Vitesse;

            if (m_Rect.x >= (GUI::getWidth() - m_Rect.w))
            {
                m_Rect.x = (GUI::getWidth() - m_Rect.w);
                m_Vitesse = -m_Vitesse / 2;
            }
            else
            {
                if (m_TimerAcceleration.verifie())
                    if (m_Vitesse < 12)
                        m_Vitesse++;
            }
        }
        else
        {
            if (m_Vitesse != 0)
            {
                m_Rect.x += m_Vitesse;

                if (m_Rect.x >= (GUI::getWidth() - m_Rect.w))
                {
                    m_Rect.x = (GUI::getWidth() - m_Rect.w);
                    m_Vitesse = -m_Vitesse / 2;
                }

                if (m_TimerGlissage.verifie())
                    m_Vitesse += (m_Vitesse > 0)?-1:((m_Vitesse < 0)?1:0);
            }
        }
    }
    else
        m_Vitesse = 0;
}

void VaisseauJoueur::attaque(bool active)
{
    if (active && getNiveauVie())
    {
        if (m_TimerAttaque.verifie())
        {
            m_ModeleMissile->setPos(m_Rect.x + ((m_Rect.w - m_ModeleMissile->getRect().w) / 2), m_Rect.y);
            s_ListeMissiles.push_back(*(m_ModeleMissile));
        }
    }
}


int VaisseauJoueur::getNiveauVie() const
{
    return (m_NiveauVie > 0)?m_NiveauVie:0;
}

void VaisseauJoueur::diminueVie(int degat)
{
    if(m_NiveauVie > degat)
    {
        setEtatAnimeActuel(5);
        m_NiveauVie -= degat;
    }
    else
        m_NiveauVie = -1;
}

void VaisseauJoueur::setEtatAnimeActuel(int etatActuel)
{
    if (m_EtatAnime != etatActuel)
    {
        if (etatActuel == 5)        // Le vaisseau est touché
        {
            m_EtatAnime = 5;
            m_ImageActuelle = 20;
            m_TimerHit.demarre();
        }

        if (m_TimerHit.getTemps() >= 150)   // 150 ms après qu'il est touché, doit retourner à l'état normal
            m_TimerHit.arrete();

        if (!m_TimerHit.estDemarre())
        {
            m_EtatAnime = etatActuel;
            switch (m_EtatAnime)
            {
            case 0:
                m_ImageActuelle = 0;
                break;
            case 1:
                m_ImageActuelle = 4;
                break;
            case 2:
                m_ImageActuelle = 8;
                break;
            case 3:
                m_ImageActuelle = 12;
                break;
            case 4:
                m_ImageActuelle = 16;
                break;
            default:
                break;
            }
        }
    }
}

void VaisseauJoueur::dessineEntite(bool dessineSeulement)
{
    if (!dessineSeulement)
    {
        if (m_Vitesse <= 12 &&  m_Vitesse > 1)
            setEtatAnimeActuel(4);

        else if (m_Vitesse <= 1 && m_Vitesse > 0)
            setEtatAnimeActuel(3);

        else if (m_Vitesse == 0)
            setEtatAnimeActuel(0);

        else if (m_Vitesse >= -1 && m_Vitesse < 0)
            setEtatAnimeActuel(2);

        else if (m_Vitesse >= -12  && m_Vitesse < -1)
            setEtatAnimeActuel(1);

        if (m_TimerAnimation.verifie() && !m_TimerHit.estDemarre())
        {
            m_ImageActuelle++;
            switch (m_EtatAnime)
            {
            case 0:
                if (m_ImageActuelle >= 4)
                    m_ImageActuelle = 0;
                break;
            case 1:
                if (m_ImageActuelle >= 8)
                    m_ImageActuelle = 4;
                break;
            case 2:
                if (m_ImageActuelle >= 12)
                    m_ImageActuelle = 8;
                break;
            case 3:
                if (m_ImageActuelle >= 16)
                    m_ImageActuelle = 12;
                break;
            case 4:
                if (m_ImageActuelle >= 20)
                    m_ImageActuelle = 16;
                break;
            default:
                break;
            }
        }

        if (m_NiveauVie == 0)
            appelNouveauVaisseau();
        else
            Entite::dessineEntite();
    }
    else
    {
        if (m_NiveauVie > 0)
            Entite::dessineEntite(dessineSeulement);
    }

}

void VaisseauJoueur::dessineMissiles(bool dessineSeulement)
{
    list<MissileJoueur>::iterator iter = s_ListeMissiles.begin();
    MissileJoueur* toDelete = NULL;

    while (iter != s_ListeMissiles.end())
    {
        if (!dessineSeulement && iter->getNiveauVie() != -1 && iter->getNiveauVie() != 0)
        {
            // Si la missile n'est pas en train d'etre explosée
            iter->testCollision();
            iter->setPos(iter->getRect().x, iter->getRect().y - 10);
        }

        iter->dessineEntite(dessineSeulement);

        if (iter->getRect().y <= -iter->getRect().h || !(iter->getNiveauVie()))
        {
            toDelete = &(*iter);
        }

        iter++;

        if (toDelete)
        {
            s_ListeMissiles.remove(*toDelete);
            toDelete = NULL;
        }
    }
}

void VaisseauJoueur::viderListeMissiles()
{
    s_ListeMissiles.clear();
}

//void VaisseauJoueur::redimenstionneMissiles()
//{
//    list<MissileJoueur>::iterator iter = s_ListeMissiles.begin();
//
//    while (iter != s_ListeMissiles.end())
//    {
//        iter->redimensionne(true);
//        iter++;
//    }
//}

void VaisseauJoueur::redimensionne(bool rechargerImages)
{
    if (rechargerImages)
    {
        // rechargement des images de taille d'origine
        SDL_FreeSurface(m_SurfaceSource);
        setFichier(m_Fichier);

        //SDL_FreeSurface(m_SurfaceSourceExplosion);
        //setFichierExplosion(m_FichierExplosion);

        m_Images.clear();
        //m_ImagesExplosion.clear();
        delete m_ModeleMissile;
        chargerImages();
    }

    Entite::redimensionne();
    setPos(m_Rect.x, GUI::getHeight() - m_Rect.h);
}

void VaisseauJoueur::appelNouveauVaisseau()
{
    static Timer attenteApparition;
    if (!attenteApparition.estDemarre())
        attenteApparition.demarre();

    if (m_VaisseauxRestants > 0 && attenteApparition.getTemps() > 2500)
    {
        attenteApparition.arrete();

        setPos((GUI::getWidth() - m_Rect.w) / 2, GUI::getHeight() - m_Rect.h);
        m_VaisseauxRestants--;
        m_NiveauVie = s_NiveauVieMax;
    }
    else if (m_VaisseauxRestants == 0)
        attenteApparition.arrete();

}

int VaisseauJoueur::getVaisseauxRestants() const
{
    return m_VaisseauxRestants;
}

VaisseauJoueur::~VaisseauJoueur()
{
    delete m_ModeleMissile;
    s_InstanceCree = false;
}
