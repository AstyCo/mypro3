#ifndef FROMBALLISTICEXTENSION_H
#define FROMBALLISTICEXTENSION_H

extern const double CMU; //gravitational constant of the Earth

double calcPeriodKeplerian (const double a);

/// @brief Расчет количества секунд из количества витков
/// @param nvit Kоличество витков
/// @param a_bpo Большая полусь, км
/// @return Kоличество секунд
int nvitToSec(const int nvit, const double a_bpo);

int secToNvit(int sec, double a_bpo);


#endif // FROMBALLISTICEXTENSION_H

