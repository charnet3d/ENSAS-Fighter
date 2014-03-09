#include "Stage.h"
#include <sprig.h>
#include "GUI.h"

using namespace std;

int Stage::s_Score = 0;

Stage::Stage(const string& ennemis, int intervalTirEnnemi, int nbrTirSimultane, const string& fichierArrierePlan)
{
    m_IntervalTirEnnemi = intervalTirEnnemi;
    m_NbrTirSimultane = nbrTirSimultane;
    m_MatriceEnnemi = MatriceEnnemi::getSingleton(ennemis, intervalTirEnnemi, 0, nbrTirSimultane);
    m_FichierArrierePlan = fichierArrierePlan;
    m_ArrierePlan = Entite::chargeImg(fichierArrierePlan);

    GUI::dessineChargement(1);

    redimensionne();
}

Stage::~Stage()
{
    SDL_FreeSurface(m_ArrierePlan);
    Ennemi::viderListeMissiles();
    MatriceEnnemi::libere();
}

MatriceEnnemi* Stage::getMatriceEnnemi() const
{
    return m_MatriceEnnemi;
}

int Stage::getIntervalTirEnnemi() const
{
    return m_IntervalTirEnnemi;
}

int Stage::getNbrTirSimultane() const
{
    return m_NbrTirSimultane;
}

void Stage::dessineArrierePlan()
{
    Entite::dessineSurface(0, 0, m_ArrierePlan, GUI::getEcran());
}

void Stage::redimensionne()
{
    if (GUI::getWidth() != 1024 || GUI::getHeight() != 768)
    {
        // redimensionnement de l'arrière plan
        SDL_FreeSurface(m_ArrierePlan);
        m_ArrierePlan = Entite::chargeImg(m_FichierArrierePlan);
        SDL_Surface* tmp_surf = m_ArrierePlan;
        m_ArrierePlan = SPG_Transform(m_ArrierePlan, 0, 0, GUI::getWidth() / 1024.f,
                                            GUI::getHeight() / 768.f, SPG_TAA);
        SDL_FreeSurface(tmp_surf);
    }
}

int Stage::getScore()
{
    return s_Score;
}

void Stage::reinistaliseScore()
{
    s_Score = 0;
}

void Stage::incrementeScore(int valeur)
{
    s_Score += valeur;
}
