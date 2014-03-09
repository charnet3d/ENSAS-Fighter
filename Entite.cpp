#include "Entite.h"
#include <SDL_image.h>
#include "SDL_collide.h"
#include <sprig.h>
#include "GUI.h"

using namespace std;

PartieImage::PartieImage(SDL_Surface* surfaceSrc, int pX, int pY, int pW, int pH)
{
    m_Rect.x = pX;
    m_Rect.y = pY;
    m_Rect.w = pW;
    m_Rect.h = pH;

    // On crée une surface transparente
    m_Surface = SPG_CreateSurface(SDL_HWSURFACE, pW, pH);

    // On copie d'abord le masque de transparence
    SPG_PushBlend(SPG_COPY_ALPHA_ONLY);
    SPG_Blit(surfaceSrc, &m_Rect, m_Surface, NULL);
    SPG_PopBlend();

    // Après on copie la surface opaque
    SPG_Blit(surfaceSrc, &m_Rect, m_Surface, NULL);
}

PartieImage::PartieImage(SDL_Surface* surfaceSrc)
{
    m_Rect.x = 0;
    m_Rect.y = 0;
    m_Rect.w = surfaceSrc->w;
    m_Rect.h = surfaceSrc->h;

    // On crée une surface transparente
    m_Surface = SPG_CreateSurface(SDL_HWSURFACE, surfaceSrc->w, surfaceSrc->h);

    // On copie d'abord le masque de transparence
    SPG_PushBlend(SPG_COPY_ALPHA_ONLY);
    SPG_Blit(surfaceSrc, NULL, m_Surface, NULL);
    SPG_PopBlend();

    // Après on copie la surface opaque
    SPG_Blit(surfaceSrc, &m_Rect, m_Surface, NULL);
}

PartieImage::PartieImage(const PartieImage& pi)
{
    m_Rect = pi.m_Rect;

    m_Surface = SPG_CopySurface(pi.m_Surface);
}

PartieImage::~PartieImage()
{
    SPG_Free(m_Surface);
}

SDL_Rect PartieImage::getRect() const
{
    return m_Rect;
}

SDL_Surface* PartieImage::getSurface() const
{
    return m_Surface;
}

void PartieImage::setSurface(SDL_Surface* surface)
{
    SPG_Free(m_Surface);
    m_Surface = surface;
    m_Rect.w = m_Surface->w;
    m_Rect.h = m_Surface->h;
}

Entite::Entite(const string& fichier, const string& fichierExplosion, int x, int y, bool anime)
    : m_TimerAnimationExplosion(50)
{
    setFichier(fichier);
    setFichierExplosion(fichierExplosion);
    m_Rect.x = x;
    m_Rect.y = y;
    m_Anime = anime;
    m_EtatAnime = 0;
    m_ImageActuelle = 0;
    m_ImageActuelleExplosion = 0;
    m_NiveauVie = 1;
}

Entite::Entite(const Entite& eCopie)
{
    setFichier(eCopie.m_Fichier);
    setFichierExplosion(eCopie.m_FichierExplosion);
    m_Rect = eCopie.m_Rect;

    for (unsigned int i = 0; i < eCopie.m_Images.size(); i++)
        m_Images.push_back(eCopie.m_Images[i]);

    for (unsigned int i = 0; i < eCopie.m_ImagesExplosion.size(); i++)
        m_ImagesExplosion.push_back(eCopie.m_ImagesExplosion[i]);

    m_ImageActuelle = eCopie.m_ImageActuelle;
    m_ImageActuelleExplosion = eCopie.m_ImageActuelle;
    m_Anime = eCopie.m_Anime;
    m_EtatAnime = eCopie.m_EtatAnime;

    m_TimerAnimationExplosion = eCopie.m_TimerAnimationExplosion;

    m_NiveauVie = eCopie.m_NiveauVie;
}

Entite::~Entite()
{
    SDL_FreeSurface(m_SurfaceSource);
    SDL_FreeSurface(m_SurfaceSourceExplosion);
    m_Images.clear();
    m_ImagesExplosion.clear();
}

void Entite::setPos(int x, int y)
{
    m_Rect.x = x;
    m_Rect.y = y;
}

SDL_Rect Entite::getRect() const
{
    return m_Rect;
}

void Entite::setNiveauVie(int niveauVie)
{
    m_NiveauVie = niveauVie;
}

int Entite::getNiveauVie() const
{
    return m_NiveauVie;
}

void Entite::setFichier(const string& fichier)
{
    if (fichier != "")
    {
        m_Fichier = fichier;
        m_SurfaceSource = chargeImg(fichier.c_str());
        m_Rect.w = m_SurfaceSource->w;
        m_Rect.h = m_SurfaceSource->h;
    }
    else
    {
        m_Rect.w = 0;
        m_Rect.h = 0;
        m_SurfaceSource = NULL;
    }
}

void Entite::setFichierExplosion(const string& fichierExplosion)
{
    if (fichierExplosion != "")
    {
        m_FichierExplosion = fichierExplosion;
        m_SurfaceSourceExplosion = chargeImg(fichierExplosion.c_str());
    }
    else
        m_SurfaceSourceExplosion = NULL;
}

void Entite::ajouterImage(int x, int y, int w, int h)
{
    // On suppose que toutes les partie de l'image on les mêmes largeur et hauteur
    // pour cela on a une seule valeur pour ces 2 attributs pour l'entité
    // Quelque soit son image actuelle
    m_Rect.w = w;
    m_Rect.h = h;

    PartieImage pm(m_SurfaceSource, x, y, w, h);
    m_Images.push_back(pm);
}

void Entite::ajouterImageExplosion(int x, int y, int w, int h)
{
    PartieImage pm(m_SurfaceSourceExplosion, x, y, w, h);
    m_ImagesExplosion.push_back(pm);
}

void Entite::redimensionne()
{
    if (GUI::getWidth() < 1024)
    {
        float widthScale = GUI::getWidth() / 1024.f;
        for (unsigned int i = 0; i < m_Images.size(); i++)
        {
            m_Images[i].setSurface(SPG_Transform(m_Images[i].getSurface(), 0, 0, widthScale, 1, 0));
        }

        for (unsigned int i = 0; i < m_ImagesExplosion.size(); i++)
        {
            m_ImagesExplosion[i].setSurface(SPG_Transform(m_ImagesExplosion[i].getSurface(), 0, 0, widthScale, 1, 0));
        }
        if (m_Images.size())
            m_Rect.w = m_Images[0].getSurface()->w;
    }

    if (GUI::getHeight() < 768)
    {
        float heightScale = GUI::getHeight() / 768.f;
        for (unsigned int i = 0; i < m_Images.size(); i++)
        {
            m_Images[i].setSurface(SPG_Transform(m_Images[i].getSurface(), 0, 0, 1, heightScale, 0));
        }

        for (unsigned int i = 0; i < m_ImagesExplosion.size(); i++)
        {
            m_ImagesExplosion[i].setSurface(SPG_Transform(m_ImagesExplosion[i].getSurface(), 0, 0, 1, heightScale,0));
        }
        if (m_Images.size())
            m_Rect.h = m_Images[0].getSurface()->h;
    }
}

void Entite::dessineEntite(bool dessineSeulement)
{
    if (!m_Anime)
        dessineSurface(m_Rect.x, m_Rect.y, m_SurfaceSource, GUI::getEcran());
    else
    {
        if (m_NiveauVie != -1 || (m_ImageActuelleExplosion <= 1 && m_NiveauVie == -1))
            dessineSurface(m_Rect.x, m_Rect.y, m_Images[m_ImageActuelle].getSurface(), GUI::getEcran());

        if (m_NiveauVie == -1 && m_ImageActuelleExplosion <= 7)
        {
            SDL_Surface* img = m_ImagesExplosion[m_ImageActuelleExplosion].getSurface();
            dessineSurface(m_Rect.x - ((img->w - m_Rect.w) / 2), m_Rect.y - ((img->h - m_Rect.h) / 2),
                           img, GUI::getEcran());
            if (!dessineSeulement && m_TimerAnimationExplosion.verifie())
            {
                m_ImageActuelleExplosion++;
                if (m_ImageActuelleExplosion > 7)
                {
                    m_ImageActuelleExplosion = 0;
                    m_NiveauVie = 0;
                }
            }

        }
    }
}

bool Entite::testCollision(Entite* e)
{
    if (!m_Anime)
        return SDL_CollidePixel(m_SurfaceSource, m_Rect.x, m_Rect.y, e->m_SurfaceSource, e->m_Rect.x, e->m_Rect.y, 1);
    else
        return SDL_CollidePixel(m_Images[m_ImageActuelle].getSurface(), m_Rect.x, m_Rect.y, e->m_Images[e->m_ImageActuelle].getSurface(), e->m_Rect.x, e->m_Rect.y, 1);
}

SDL_Surface* Entite::chargeImg(const std::string& fichier, bool redimensionne, bool lisser)
{
    SDL_Surface* surfaceChargee = NULL;
    SDL_Surface* surfaceOptimisee = NULL;

    surfaceChargee = IMG_Load(fichier.c_str());

    if (surfaceChargee)
    {
        if (surfaceChargee->format->BitsPerPixel < 32)
        {
            surfaceOptimisee = SDL_DisplayFormat(surfaceChargee);
            SDL_FreeSurface(surfaceChargee);
        }
        else
            surfaceOptimisee = surfaceChargee;

        if (redimensionne)
        {
            SDL_Surface* tmp_surf = surfaceOptimisee;
            surfaceOptimisee = SPG_Transform(surfaceOptimisee, 0, 0, GUI::getWidth() / 1024.f,
                                                GUI::getHeight() / 768.f, (lisser?SPG_TAA:0));
            SDL_FreeSurface(tmp_surf);
        }
    }
    else
        cerr << "Erreur: impossible de charger '" << fichier << "'" << endl;


    return surfaceOptimisee;
}

void Entite::dessineSurface(int x, int y, SDL_Surface* source, SDL_Surface* destination)
{
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;

    //SPG_Rect(GUI::getEcran(), x, y, x + source->w, y + source->h, SDL_MapRGB(GUI::getEcran()->format, 255, 255, 255));
    SDL_BlitSurface(source, NULL, destination, &pos);
}
