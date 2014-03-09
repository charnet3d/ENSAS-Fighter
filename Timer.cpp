#include "Timer.h"
#include <SDL.h>

Timer::Timer()
{
    // Initialisation des variables
    m_Demarre = false;
    m_EnPause = false;

    m_TempsDepart = 0;
    m_TempsPause = 0;
}

void Timer::demarre()
{
    // Commence le timer
    m_Demarre = true;
    m_EnPause = false;

    // On prend le temps actuel de l'horloge de l'ordinateur
    m_TempsDepart = SDL_GetTicks();
}

void Timer::arrete()
{
    // Arrète le timer
    m_Demarre = false;
    m_EnPause = false;

    m_TempsDepart = 0;
    m_TempsPause = 0;
}

void Timer::pause()
{
    // Si le timer est déja en marche et n'est pas en pause
    if(m_Demarre && !m_EnPause)
    {
        // met le timer en pause
        m_EnPause = true;

        // en calcule le temps de pause
        m_TempsPause = SDL_GetTicks() - m_TempsDepart;
    }
}

void Timer::unpause()
{
    // Si le timer est en pause
    if(m_EnPause)
    {
        // on le remet en marche
        m_EnPause = false;

        // On remet le temps de départ
        m_TempsDepart = SDL_GetTicks() - m_TempsPause;

        // et on réinitialise le temps de pause à 0
        m_TempsPause = 0;
    }
}


int Timer::getTemps() const
{
    // Si le timer fonctionne
    if (m_Demarre)
    {
        if (m_EnPause)
            return m_TempsPause;                        // On retourne l'accumulation du temps calculé lors de la pause
        else
            return SDL_GetTicks() - m_TempsDepart;       // On retourne l'accumulation du temps depuis le début
    }
    else
        return 0;           // Si le timer est arreté
}


bool Timer::estDemarre() const
{
    return m_Demarre;
}

bool Timer::estEnPause() const
{
    return m_EnPause;
}
