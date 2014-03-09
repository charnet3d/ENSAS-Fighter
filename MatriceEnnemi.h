#ifndef _MATRICEENNEMIS_H_
#define _MATRICEENNEMIS_H_

#include "LigneEnnemi.h"

class MatriceEnnemi
{
private:
    static bool s_InstanceCree;
    static MatriceEnnemi* s_Instance;

    int m_Taille;
    int m_IntervalTirEnnemi;
    int m_IntervalDescenteEnnemi;
    int m_NbrTirEnnemiSimultane;
    std::list<LigneEnnemi> m_ListeLignesEnnemis;

    TimerInterval m_TimerMouvementEnnemi, m_TimerMissiles;

    int nbrLignes(const std::string& str);
    int getNbrEnnemis();

    MatriceEnnemi(const std::string& strMatriceEnnemis = "", int intervalTirEnnemi = 0, int intervalDescenteEnnemi = 0, int nbrTirEnnemiSimultane = 0);
public:
    static MatriceEnnemi* getSingleton(const std::string& strMatriceEnnemis = "", int intervalTirEnnemi = 0, int intervalDescenteEnnemi = 0, int nbrTirEnnemiSimultane = 0);
    static void libere();
    ~MatriceEnnemi();

    LigneEnnemi& operator[](unsigned int i);
    LigneEnnemi operator[](unsigned int i) const;

    int getTailleLignes() const;

    void bougeEnnemis();
    void tirEnnemis();
    void dessineEnnemis(bool dessineSeulement = false);
//    void redimensionne();
};

#endif // _MATRICEENNEMIS_H_
