#include "TimerInterval.h"

TimerInterval::TimerInterval(int interval) : Timer(), m_Interval(interval)
{
    demarre();
    m_DernierTemps = getTemps();
}

bool TimerInterval::verifie()
{
    if (m_Interval && ((getTemps() - m_DernierTemps) > m_Interval))
    {
        m_DernierTemps = getTemps();
        return true;
    }
    else
        return false;
}
