#ifndef _MISSILEJOUEUR_H_
#define _MISSILEJOUEUR_H_

#include "Missile.h"

class MissileJoueur : public Missile
{
public:
    MissileJoueur(const std::string& fichier = "", int x = 0, int y = 0, int partieImageWidth = 0, int partieImageHeight = 0);
    MissileJoueur(const MissileJoueur& mCopie);

    virtual bool operator==(const MissileJoueur& mj) const;

    virtual void testCollision();
};

#endif // _MISSILEJOUEUR_H_
