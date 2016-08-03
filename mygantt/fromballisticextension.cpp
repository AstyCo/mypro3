
#include <QtCore/qmath.h>

const double CMU = 398600.4418; //gravitational constant of the Earth

double calcPeriodKeplerian(const double a)
{
    return 2*M_PI*qSqrt(a*a*a/CMU);
}


int nvitToSec(const int nvit, const double a_bpo)
{
    return calcPeriodKeplerian(a_bpo)*nvit;
}


int secToNvit(int sec, double a_bpo)
{
    return sec/calcPeriodKeplerian(a_bpo);
}
