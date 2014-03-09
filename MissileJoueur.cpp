#include "MatriceEnnemi.h"
#include "VaisseauJoueur.h"
#include "MissileJoueur.h"

MissileJoueur::MissileJoueur(const std::string& fichier, int x, int y, int partieImageWidth, int partieImageHeight)
    : Missile(fichier, x, y, partieImageWidth, partieImageHeight) {}
MissileJoueur::MissileJoueur(const MissileJoueur& mCopie) : Missile(mCopie) {}

bool MissileJoueur::operator==(const MissileJoueur& mj) const
{
    if (this == &mj)
        return true;
    else
        return false;
}

// Parcours la matrice d'ennemis et teste la collision de la missile actuelle avec eux
void MissileJoueur::testCollision()
{
    MatriceEnnemi* matrice = MatriceEnnemi::getSingleton();

    LigneEnnemi* ligne = NULL;
    Ennemi* ennemi = NULL;
    int tailleColonnes = 0;

    for (int i = 0; i < matrice->getTailleLignes(); i++)
    {
        ligne = &((*matrice)[i]);
        tailleColonnes = ligne->getTaille();
        for (int j = 0; j < tailleColonnes; j++)
        {
            ennemi = &((*ligne)[j]);
            if (Entite::testCollision(ennemi))
            {
                ennemi->diminueVie(1);
                setNiveauVie(0);        // le niveau de vie de la missile en cours
            }
        }
    }
}
