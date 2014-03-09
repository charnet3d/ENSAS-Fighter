#include "Ennemi.h"
#include "GUI.h"
#include "Stage.h"

using namespace std;

std::list<MissileEnnemi> Ennemi::s_ListeMissiles;

void Ennemi::chargerImages()
{
    int partieImageWidth, partieImageHeight;

    if (m_Type == "E2")
    {
        setFichier("Media\\ennemi2.png");
        setFichierExplosion("Media\\ExplosionMoyenne.png");

        partieImageWidth = 40;
        partieImageHeight = 57;
        for (int i = 0; i < 5; i++)
            ajouterImage(partieImageWidth * i + i, 0, partieImageWidth, partieImageHeight);

        m_ModeleMissile = new MissileEnnemi("Media\\MissileEnnemi2.png", 0, 0, 11, 31);
    }
    else if (m_Type == "E3")
    {
        setFichier("Media\\ennemi3.png");
        setFichierExplosion("Media\\ExplosionMoyenne.png");

        partieImageWidth = 94;
        partieImageHeight = 57;
        for (int i = 0; i < 5; i++)
            ajouterImage(partieImageWidth * i + i, 0, partieImageWidth, partieImageHeight);

        m_ModeleMissile = new MissileEnnemi("Media\\MissileEnnemi1.png", 0, 0, 11, 31);
    }
    else if (m_Type == "E4")
    {
        setFichier("Media\\ennemi4.png");
        setFichierExplosion("Media\\ExplosionMoyenne.png");

        partieImageWidth = 94;
        partieImageHeight = 57;
        for (int i = 0; i < 5; i++)
            ajouterImage(partieImageWidth * i + i, 0, partieImageWidth, partieImageHeight);

        m_ModeleMissile = new MissileEnnemi("Media\\MissileEnnemi2.png", 0, 0, 11, 31);
    }
    else if (m_Type == "E5")
    {
        setFichier("Media\\ennemi5.png");
        setFichierExplosion("Media\\ExplosionMoyenne.png");

        partieImageWidth = 94;
        partieImageHeight = 100;
        for (int i = 0; i < 5; i++)
            ajouterImage(partieImageWidth * i + i, 0, partieImageWidth, partieImageHeight);

        m_ModeleMissile = new MissileEnnemi("Media\\MissileEnnemi1.png", 0, 0, 11, 31);
    }
    else if (m_Type == "E6")
    {
        setFichier("Media\\ennemi6.png");
        setFichierExplosion("Media\\ExplosionMoyenne.png");

        partieImageWidth = 94;
        partieImageHeight = 100;
        for (int i = 0; i < 5; i++)
            ajouterImage(partieImageWidth * i + i, 0, partieImageWidth, partieImageHeight);

        m_ModeleMissile = new MissileEnnemi("Media\\MissileEnnemi2.png", 0, 0, 11, 31);
    }
    else if (m_Type == "E7")
    {
        setFichier("Media\\ennemi7.png");
        setFichierExplosion("Media\\ExplosionGrande.png");

        partieImageWidth = 189;
        partieImageHeight = 131;
        for (int i = 0; i < 2; i++)
            ajouterImage(partieImageWidth * i + i, 0, partieImageWidth, partieImageHeight);

        m_ModeleMissile = new MissileEnnemi("Media\\MissileEnnemi1.png", 0, 0, 11, 31);
    }
    else if (m_Type == "E8")
    {
        setFichier("Media\\ennemi8.png");
        setFichierExplosion("Media\\ExplosionGrande.png");

        partieImageWidth = 189;
        partieImageHeight = 131;
        for (int i = 0; i < 2; i++)
            ajouterImage(partieImageWidth * i + i, 0, partieImageWidth, partieImageHeight);

        m_ModeleMissile = new MissileEnnemi("Media\\MissileEnnemi1.png", 0, 0, 11, 31);
    }
    else if (m_Type == "E9")
    {
        setFichier("Media\\ennemi9.png");
        setFichierExplosion("Media\\ExplosionGrande.png");

        partieImageWidth = 189;
        partieImageHeight = 131;
        for (int i = 0; i < 2; i++)
            ajouterImage(partieImageWidth * i + i, 0, partieImageWidth, partieImageHeight);

        m_ModeleMissile = new MissileEnnemi("Media\\MissileEnnemi2.png", 0, 0, 11, 31);
    }
    else if (m_Type == "E10")
    {
        setFichier("Media\\ennemi10.png");
        setFichierExplosion("Media\\ExplosionGrande.png");

        partieImageWidth = 189;
        partieImageHeight = 131;
        for (int i = 0; i < 2; i++)
            ajouterImage(partieImageWidth * i + i, 0, partieImageWidth, partieImageHeight);

        m_ModeleMissile = new MissileEnnemi("Media\\MissileEnnemi2.png", 0, 0, 11, 31);
    }
    else if (m_Type == "E11")
    {
        setFichier("Media\\ennemi11.png");
        setFichierExplosion("Media\\ExplosionGrande.png");

        partieImageWidth = 189;
        partieImageHeight = 131;
        for (int i = 0; i < 2; i++)
            ajouterImage(partieImageWidth * i + i, 0, partieImageWidth, partieImageHeight);

        m_ModeleMissile = new MissileEnnemi("Media\\MissileEnnemi2.png", 0, 0, 11, 31);
    }
    else        // cas de "E1" inclus
    {
        setFichier("Media\\ennemi1.png");
        setFichierExplosion("Media\\ExplosionMoyenne.png");

        partieImageWidth = 40;
        partieImageHeight = 57;
        for (int i = 0; i < 5; i++)
            ajouterImage(partieImageWidth * i + i, 0, partieImageWidth, partieImageHeight);

        m_ModeleMissile = new MissileEnnemi("Media\\MissileEnnemi1.png", 0, 0, 11, 31);
    }

    partieImageWidth = (m_SurfaceSourceExplosion->w - 3) / 4;
    partieImageHeight = (m_SurfaceSourceExplosion->h - 1) / 2;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 4; j++)
            ajouterImageExplosion(partieImageWidth * j + j, partieImageHeight * i + i, partieImageWidth, partieImageHeight);

}

//constructeur principale
Ennemi::Ennemi(int x, int y, const string& type)
    : Entite("", "", x, y, true), m_TimerAnimation(120), m_TimerHit()
{

    m_Type = type;

    chargerImages();

    // initialisation du niveau de vie
    if (type == "E2")
    {
        m_ScoreRecompense = 200;
        m_NiveauVie = 2;
    }
    else if (type == "E3")
    {
        m_ScoreRecompense = 300;
        m_NiveauVie = 3;
    }
    else if (type == "E4")
    {
        m_ScoreRecompense = 400;
        m_NiveauVie = 4;
    }
    else if (type == "E5")
    {
        m_ScoreRecompense = 500;
        m_NiveauVie = 5;
    }
    else if (type == "E6")
    {
        m_ScoreRecompense = 600;
        m_NiveauVie = 6;
    }
    else if (type == "E7")
    {
        m_ScoreRecompense = 700;
        m_NiveauVie = 7;
    }
    else if (type == "E8")
    {
        m_ScoreRecompense = 800;
        m_NiveauVie = 8;
    }
    else if (type == "E9")
    {
        m_ScoreRecompense = 900;
        m_NiveauVie = 9;
    }
    else if (type == "E10")
    {
        m_ScoreRecompense = 1000;
        m_NiveauVie = 10;
    }
    else if (type == "E11")
    {
        m_ScoreRecompense = 1100;
        m_NiveauVie = 11;
    }
    else        // cas de "E1" inclus
    {
        m_ScoreRecompense = 100;
        m_NiveauVie = 1;
    }


    // On redimensionne les images selon la résolution de l'écran actuelle
    redimensionne();

    setEtatAnimeActuel(0);
}

Ennemi::Ennemi(const Ennemi& eCopie) : Entite(eCopie)
{
    m_Type = eCopie.m_Type;
    m_NiveauVie = eCopie.m_NiveauVie;
    m_ModeleMissile = new MissileEnnemi(*(eCopie.m_ModeleMissile));
    m_TimerAnimation = eCopie.m_TimerAnimation;

    m_ScoreRecompense = eCopie.m_ScoreRecompense;
}

Ennemi::~Ennemi()
{
    delete m_ModeleMissile;

}

bool Ennemi::operator==(const Ennemi& e) const
{
    if (this == &e)
        return true;
    else
        return false;
}

int Ennemi::getScoreRecompense() const
{
    return m_ScoreRecompense;
}

void Ennemi::attaque()
{
    if (m_Type == "E11")
    {
        // Missile de gauche
        m_ModeleMissile->setPos(m_Rect.x, m_Rect.y + m_Rect.h - (int)(73 * (GUI::getHeight() / 768.f)));
        s_ListeMissiles.push_back(*(m_ModeleMissile));
        // Missile du milieu
        m_ModeleMissile->setPos(m_Rect.x + ((m_Rect.w - m_ModeleMissile->getRect().w) / 2), m_Rect.y + m_Rect.h - (int)(40 * (GUI::getHeight() / 768.f)));
        s_ListeMissiles.push_back(*(m_ModeleMissile));
        // Missile de droite
        m_ModeleMissile->setPos(m_Rect.x + m_Rect.w - m_ModeleMissile->getRect().w, m_Rect.y + m_Rect.h - (int)(73 * (GUI::getHeight() / 768.f)));
        s_ListeMissiles.push_back(*(m_ModeleMissile));
    }
    else if (m_Type == "E10" ||
             m_Type == "E9" ||
             m_Type == "E8" ||
             m_Type == "E7")
    {
        // Missile de gauche
        m_ModeleMissile->setPos(m_Rect.x + (int)(27 * (GUI::getWidth() / 1024.f)), m_Rect.y + m_Rect.h - (int)(93 * (GUI::getHeight() / 768.f)));
        s_ListeMissiles.push_back(*(m_ModeleMissile));
        // Missile de droite
        m_ModeleMissile->setPos(m_Rect.x + m_Rect.w - m_ModeleMissile->getRect().w - (int)(27 * (GUI::getWidth() / 1024.f)), m_Rect.y + m_Rect.h - (int)(93 * (GUI::getHeight() / 768.f)));
        s_ListeMissiles.push_back(*(m_ModeleMissile));
    }
    else
    {
        // Missile du milieu
        m_ModeleMissile->setPos(m_Rect.x + ((m_Rect.w - m_ModeleMissile->getRect().w) / 2), m_Rect.y + m_Rect.h - (int)(40 * (GUI::getHeight() / 768.f)));
        s_ListeMissiles.push_back(*(m_ModeleMissile));
    }
}

void Ennemi::diminueVie(int degat)
{
    if(m_NiveauVie > degat)
    {
        setEtatAnimeActuel(1);
        m_NiveauVie -= degat;
    }
    else
    {
        m_NiveauVie = -1;
        Stage::incrementeScore(m_ScoreRecompense);
    }
}

void Ennemi::setEtatAnimeActuel(int etatActuel)
{
    if (m_EtatAnime != etatActuel)
    {
        if (etatActuel == 1)        // L'ennemi est touché
        {
            m_EtatAnime = 1;
            m_ImageActuelle = m_Images.size() - 1;  // la dernière est celle qui représente l'ennemi touché
            m_TimerHit.demarre();
        }

        if (m_TimerHit.getTemps() >= 150)    // 150 ms après qu'il est touché, doit retourner à l'état normal
            m_TimerHit.arrete();

        if (!m_TimerHit.estDemarre())
        {
            m_EtatAnime = etatActuel;
            m_ImageActuelle = 0;        // Le seul cas possible est l'état 0 (l'état normal)
        }
    }
}

void Ennemi::dessineEntite(bool dessineSeulement)
{
    if (!dessineSeulement)
    {
        setEtatAnimeActuel(0);
        if (m_TimerAnimation.verifie() && !m_TimerHit.estDemarre())
        {
            m_ImageActuelle++;
            if (m_ImageActuelle >= (int)(m_Images.size() - 1))
                m_ImageActuelle = 0;
        }
    }

    Entite::dessineEntite(dessineSeulement);
}

void Ennemi::dessineMissiles(bool dessineSeulement)
{
    list<MissileEnnemi>::iterator iter = s_ListeMissiles.begin();
    MissileEnnemi* toDelete = NULL;

    while (iter != s_ListeMissiles.end())
    {
        if (!dessineSeulement && iter->getNiveauVie() != -1 && iter->getNiveauVie() != 0)
        {
            // Si la missile n'est pas en train d'etre explosée
            iter->testCollision();
            iter->setPos(iter->getRect().x, iter->getRect().y + 10);
        }

        iter->dessineEntite(dessineSeulement);

        if (iter->getRect().y >= GUI::getHeight() + iter->getRect().h || !(iter->getNiveauVie()))
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

void Ennemi::viderListeMissiles()
{
    s_ListeMissiles.clear();
}

//void Ennemi::redimenstionneMissiles()
//{
//    list<MissileEnnemi>::iterator iter = s_ListeMissiles.begin();
//
//    while (iter != s_ListeMissiles.end())
//    {
//        iter->redimensionne(true);
//        iter++;
//    }
//}


void Ennemi::redimensionne(bool rechargerImages)
{
    if (rechargerImages)
    {
        // rechargement des images de taille d'origine
        SDL_FreeSurface(m_SurfaceSource);
        setFichier(m_Fichier);

        SDL_FreeSurface(m_SurfaceSourceExplosion);
        setFichierExplosion(m_FichierExplosion);

        m_Images.clear();
        m_ImagesExplosion.clear();
        delete m_ModeleMissile;
        chargerImages();
    }

    Entite::redimensionne();
}
