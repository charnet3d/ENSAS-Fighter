#ifndef _Missile_H_
#define _Missile_H_

#include "Entite.h"

class Missile : public Entite
{
protected:
    TimerInterval m_TimerAnimation;

    int m_PartieImageWidth;
    int m_PartieImageHeight;

    void chargerImages();
public:
    Missile(const std::string& fichier = "", int x = 0, int y = 0, int partieImageWidth = 0, int partieImageHeight = 0);
    Missile(const Missile& mCopie);

    virtual bool operator==(const Missile& m) const;

    void redimensionne(bool rechargerImages = false);
    virtual void setNiveauVie(int niveauVie);
    virtual void dessineEntite(bool dessineSeulement = false);
    virtual void testCollision() = 0;
};

#endif
