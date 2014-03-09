#ifndef _STAGE_H_
#define _STAGE_H_

#include "MatriceEnnemi.h"

class Stage
{
private:
    MatriceEnnemi* m_MatriceEnnemi;
    int m_IntervalTirEnnemi;
    int m_NbrTirSimultane;
    SDL_Surface* m_ArrierePlan;
    std::string m_FichierArrierePlan;

    static int s_Score;
public:
    Stage(const std::string& ennemis, int intervalTirEnnemi, int nbrTirSimultane, const std::string& fichierArrierePlan);
    ~Stage();

    MatriceEnnemi* getMatriceEnnemi() const;
    int getIntervalTirEnnemi() const;
    int getNbrTirSimultane() const;
    void dessineArrierePlan();
    void redimensionne();

    static int getScore();
    static void reinistaliseScore();
    static void incrementeScore(int valeur);
};

#endif // _STAGE_H_
