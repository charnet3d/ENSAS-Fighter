#ifndef _MISSILEENNEMI_H_
#define _MISSILEENNEMI_H_

#include "Missile.h"

class MissileEnnemi : public Missile
{
public:
    MissileEnnemi(const std::string& fichier = "", int x = 0, int y = 0, int partieImageWidth = 0, int partieImageHeight = 0);
    MissileEnnemi(const MissileEnnemi& mCopie);

    virtual bool operator==(const MissileEnnemi& mj) const;

    virtual void testCollision();
};

#endif // _MISSILEENNEMI_H_
