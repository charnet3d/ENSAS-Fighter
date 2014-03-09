#ifndef _LIGNEENNEMI_H_
#define _LIGNEENNEMI_H_

#include "Ennemi.h"

class LigneEnnemi
{
private:
    bool m_DirectionHoriz;
    int m_YDepart;
    std::list<Ennemi> m_ListeEnnemis;
    int m_Taille;

    int nbrMots(const std::string& str);

public:
    LigneEnnemi(const std::string& strLigneEnnemis = "", bool directionHoriz = true, int yDepart = 0);

    // surcharge de l'opérateur []
    Ennemi& operator[](unsigned int i);
    Ennemi operator[](unsigned int i) const;

    bool operator==(const LigneEnnemi& le) const;

    int getTaille() const;
//    void setYDepart(int yDepart);

    void descendre();
    void bougeHoriz();
    void tirEnnemis();
    void dessineEnnemis(bool dessineSeulement = false);
//    void redimensionne();
};

#endif // _LIGNEENNEMI_H_
