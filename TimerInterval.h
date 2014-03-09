#ifndef _TimerInterval_H_
#define _TimerInterval_H_

#include "Timer.h"

class TimerInterval : public Timer
{
private:
    // Le temps de d�but en milliseconde
    int m_DernierTemps;
    int m_Interval;

public:

    // Constructeur pour l'initialisation
    TimerInterval(int interval = 1000);

    // Si on est arriv� � l'interval voulu, on actualise le dernier temps pour une prochaine v�rification
    bool verifie();
};


#endif // _TimerInterval_H_
