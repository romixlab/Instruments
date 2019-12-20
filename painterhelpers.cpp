#include "painterhelpers.h"
#include <cmath>

qreal instruments::wraphalf(qreal x, qreal around)
{
    qreal intpart;
    if (around == 0)
        return x;
    if (x > around) {
        modf(x / around, &intpart);
        x -= intpart * around;
    }
    if (x < 0) {
        modf(x / around, &intpart);
        x += (intpart + 1) * around;
    }
    return x;
}
