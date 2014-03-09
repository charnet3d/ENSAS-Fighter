#ifndef _SIMPLETIMER_H_
#define _SIMPLETIMER_H_

class Timer
{
protected:
    // Le temps de d�but en milliseconde
    int m_TempsDepart;
    int m_TempsPause;

    // L'�tat du timer
    bool m_Demarre;
    bool m_EnPause;

public:

    // Constructeur pour l'initialisation
    Timer();

    // Les diff�rentes action du timer
    void demarre();
    void arrete();
    void pause();
    void unpause();

    // Retourne le temps indiqu� dans le timer
    int getTemps() const;

    // V�rifie l'�tat du timer
    bool estDemarre() const;
    bool estEnPause() const;
};

#endif // _SIMPLETIMER_H_
