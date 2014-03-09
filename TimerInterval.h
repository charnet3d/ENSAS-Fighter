#ifndef _TimerInterval_H_
#define _TimerInterval_H_

#include "Timer.h"

class TimerInterval : public Timer
{
private:
    // Le temps de début en milliseconde
    int m_DernierTemps;
    int m_Interval;

public:

    // Constructeur pour l'initialisation
    TimerInterval(int interval = 1000);

    // Si on est arrivé à l'interval voulu, on actualise le dernier temps pour une prochaine vérification
    bool verifie();
};


#endif // _TimerInterval_H_
