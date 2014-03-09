#ifndef _VaisseauJoueur_H_
#define _VaisseauJoueur_H_

#include <list>
#include "MissileJoueur.h"

class VaisseauJoueur : public Entite
{
private:
    static bool s_InstanceCree;
    static VaisseauJoueur* s_Instance;

    // Vitesse avec laquelle le vaisseu bouge
    int m_Vitesse;

    // Les timers utilisés pour le mouvement, l'accéleration, le glissage, ainsi que pour controler la vitesse de tir
    TimerInterval m_TimerAcceleration, m_TimerGlissage, m_TimerAttaque, m_TimerAnimation;
    Timer m_TimerHit;

    int m_PartieImageWidth;
    int m_PartieImageHeight;

    // Le modèle de projectile (Missile) en cours d'utilisation par le vaisseau
    MissileJoueur* m_ModeleMissile;

    // La liste des projectiles tirés et encore en l'air
    static std::list<MissileJoueur> s_ListeMissiles;

    int m_VaisseauxRestants;

    // Constructeur
    VaisseauJoueur(const std::string& fichier, int x, int y, int partieImageWidth, int partieImageHeight);
    void chargerImages();

public:
    static int s_NiveauVieMax;

    static VaisseauJoueur* getSingleton(const std::string& fichier = "", int x = 0, int y = 0, int partieImageWidth = 0, int partieImageHeight = 0);
    static void libere();
    virtual ~VaisseauJoueur();

    void bougeGauche(bool active);
    void bougeDroite(bool active);
    void attaque(bool active);

    int getNiveauVie() const;
    void redimensionne(bool rechargerImages = false);
    void setEtatAnimeActuel(int etatActuel);
    virtual void dessineEntite(bool dessineSeulement = false);
    static void dessineMissiles(bool dessineSeulement = false);
    //static void redimenstionneMissiles();
    static void viderListeMissiles();
    void diminueVie(int degat);
    void appelNouveauVaisseau();
    int getVaisseauxRestants() const;
};

#endif // _VaisseauJoueur_H_
