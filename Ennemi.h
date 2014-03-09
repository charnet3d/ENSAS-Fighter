#ifndef _ENNEMI_H_
#define _ENNEMI_H_

#include <list>
#include "Entite.h"
#include "MissileEnnemi.h"

/*
cette clase represente l'ensemble des ennemis
qui se trouvent en haut de l'ecran. Ces derniers
seront l'objectif de chaque joueur.
*/
class Ennemi : public Entite
{
private:
    std::string m_Type;
    MissileEnnemi* m_ModeleMissile;

    // La liste des projectiles tirés et encore en l'air
    static std::list<MissileEnnemi> s_ListeMissiles;

    TimerInterval m_TimerAnimation;
    Timer m_TimerHit;

    int m_ScoreRecompense;

    void chargerImages();

public:
    Ennemi(int x = 0, int y = 0, const std::string& type = "E1");
    Ennemi(const Ennemi& eCopie);
    virtual ~Ennemi();

    bool operator==(const Ennemi& e) const;

    int getScoreRecompense() const;

    void redimensionne(bool rechargerImages = false);
    void setEtatAnimeActuel(int etatActuel);
    virtual void dessineEntite(bool dessineSeulement = false);
    static void dessineMissiles(bool dessineSeulement = false);
    //static void redimenstionneMissiles();
    static void viderListeMissiles();
    void attaque();
    void diminueVie(int degat);
};
#endif // _ENNEMI_H_
