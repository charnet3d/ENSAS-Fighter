#ifndef _ENTREESORTIE_H_
#define _ENTREESORTIE_H_

#include "VaisseauJoueur.h"
#include "Stage.h"

class EntreeSortie
{
public:
    static bool initialiseSDL();
    static void EnregistreOptions(int widthEcran, int heightEcran, bool pleinEcran);
    static Stage* chargeStage(int numStage, bool* stageExist = NULL);
    static VaisseauJoueur* chargeJoueur();
    static void libere();
};

#endif // _ENTREESORTIE_H_
