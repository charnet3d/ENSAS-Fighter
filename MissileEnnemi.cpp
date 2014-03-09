#include "MissileEnnemi.h"
#include "VaisseauJoueur.h"

MissileEnnemi::MissileEnnemi(const std::string& fichier, int x, int y, int partieImageWidth, int partieImageHeight)
    : Missile(fichier, x, y, partieImageWidth, partieImageHeight)    {}
MissileEnnemi::MissileEnnemi(const MissileEnnemi& mCopie) : Missile(mCopie) {}

bool MissileEnnemi::operator==(const MissileEnnemi& mj) const
{
    if (this == &mj)
        return true;
    else
        return false;
}

void MissileEnnemi::testCollision()
{
    VaisseauJoueur* vaisseau = VaisseauJoueur::getSingleton();

    if (Entite::testCollision(vaisseau) && vaisseau->getNiveauVie())
    {
        vaisseau->diminueVie(10);
        setNiveauVie(0);        // le niveau de vie de la missile en cours
    }
}
