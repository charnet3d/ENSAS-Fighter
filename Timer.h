#ifndef _SIMPLETIMER_H_
#define _SIMPLETIMER_H_

class Timer
{
protected:
    // Le temps de début en milliseconde
    int m_TempsDepart;
    int m_TempsPause;

    // L'état du timer
    bool m_Demarre;
    bool m_EnPause;

public:

    // Constructeur pour l'initialisation
    Timer();

    // Les différentes action du timer
    void demarre();
    void arrete();
    void pause();
    void unpause();

    // Retourne le temps indiqué dans le timer
    int getTemps() const;

    // Vérifie l'état du timer
    bool estDemarre() const;
    bool estEnPause() const;
};

#endif // _SIMPLETIMER_H_
