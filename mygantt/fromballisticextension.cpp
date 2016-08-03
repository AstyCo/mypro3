
#include <QtCore/qmath.h>

const double CMU = 398600.4418; //gravitational constant of the Earth

double calcPeriodKeplerian(const double a)
{
    return 2*M_PI*qSqrt(a*a*a/CMU);
}
