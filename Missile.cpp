#include "Missile.h"

using namespace std;

void Missile::chargerImages()
{
    for (int i = 0; i < 4; i++)
        ajouterImage(m_PartieImageWidth * i + i, 0, m_PartieImageWidth, m_PartieImageHeight);

    int partieImageWidth = (m_SurfaceSourceExplosion->w - 3) / 4;
    int partieImageHeight = (m_SurfaceSourceExplosion->h - 1) / 2;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 4; j++)
            ajouterImageExplosion(partieImageWidth * j + j, partieImageHeight * i + i, partieImageWidth, partieImageHeight);
}

Missile::Missile(const string& fichier, int x, int y, int partieImageWidth, int partieImageHeight)
    : Entite(fichier, "", x, y, true), m_TimerAnimation(80)
{
    m_PartieImageWidth = partieImageWidth;
    m_PartieImageHeight = partieImageHeight;

    setFichierExplosion("Media\\ExplosionPetite.png");

    chargerImages();

    // On redimensionne les images selon la résolution de l'écran actuelle
    redimensionne();
}

Missile::Missile(const Missile& mCopie) : Entite(mCopie)
{
    m_TimerAnimation = mCopie.m_TimerAnimation;

    m_PartieImageWidth = mCopie.m_PartieImageWidth;
    m_PartieImageHeight = mCopie.m_PartieImageHeight;
}

bool Missile::operator==(const Missile& b) const
{
    if (this == &b)
        return true;
    else
        return false;
}

void Missile::setNiveauVie(int niveauVie)
{
    if (niveauVie > 0)
        m_NiveauVie = 1;
    else
        m_NiveauVie = -1;
}

void Missile::dessineEntite(bool dessineSeulement)
{
    if (!dessineSeulement && m_TimerAnimation.verifie())
    {
        m_ImageActuelle++;
        if (m_ImageActuelle >= 4)
            m_ImageActuelle = 0;
    }

    Entite::dessineEntite(dessineSeulement);
}

void Missile::redimensionne(bool rechargerImages)
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
        chargerImages();
    }

    Entite::redimensionne();
}
