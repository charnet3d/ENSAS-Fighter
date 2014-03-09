#ifndef _ENTITE_H_
#define _ENTITE_H_

#include <iostream>
#include <SDL.h>
#include <vector>
#include "TimerInterval.h"

class PartieImage
{
private:
    SDL_Rect m_Rect;
    SDL_Surface* m_Surface;

public:
    PartieImage(SDL_Surface* surfaceSrc, int pX, int pY, int pW, int pH);
    PartieImage(SDL_Surface* surfaceSrc);
    PartieImage(const PartieImage& pi);
    ~PartieImage();

    SDL_Rect getRect() const;
    SDL_Surface* getSurface() const;
    void setSurface(SDL_Surface* surface);
};

class Entite
{
protected:
    std::string m_Fichier;
    SDL_Surface* m_SurfaceSource;
    std::string m_FichierExplosion;
    SDL_Surface* m_SurfaceSourceExplosion;

    int m_ImageActuelle;
    int m_ImageActuelleExplosion;
    std::vector<PartieImage> m_Images;
    std::vector<PartieImage> m_ImagesExplosion;
    SDL_Rect m_Rect;
    bool m_Anime;
    int m_EtatAnime;

    TimerInterval m_TimerAnimationExplosion;

    int m_NiveauVie;

    void setFichier(const std::string& fichier);
    void setFichierExplosion(const std::string& fichierExplosion);

public:
    Entite(const std::string& fichier = "", const std::string& fichierExplosion = "", int x = 0, int y = 0, bool anime = false);
    Entite(const Entite& eCopie);
    virtual ~Entite();

    void setPos(int x, int y);
    SDL_Rect getRect() const;

    virtual void setNiveauVie(int niveauVie);
    int getNiveauVie() const;

    void ajouterImage(int x, int y, int w, int h);
    void ajouterImageExplosion(int x, int y, int w, int h);
    void redimensionne();

    // cette methode dessinera l'entité seulement si celle-ci ne contient pas d'animation
    // dans le cas contraire on va devoir la redéfinir dans la classe fille qui va implémenter
    // la façon avec laquelle on dessine (selon les états d'animation - normale, explosion..)
    virtual void dessineEntite(bool dessineSeulement = false);
    bool testCollision(Entite* e);

    static SDL_Surface* chargeImg(const std::string& fichier, bool redimensionne = false, bool lisser = true);
    static void dessineSurface(int x, int y, SDL_Surface* source, SDL_Surface* destination);
};

#endif // ENTITE_H_
