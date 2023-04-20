#ifndef CARNOT_H
#define CARNOT_H

#include <QtCharts>
#include "common.h"
#include "cycle.h"

struct Carnot: Cycle
{
    Carnot()
    {
        series_pv->setName("Carnot");
        series_ts->setName("Carnot");
    }

    void processes()
    {
        isentropic_compression(0, P, V, T, work);
        isothermal_heat_rejection(1, V[nquad-1], VH, P, V, T, work);
        isentropic_expansion(2, P, V, T, work);
        isothermal_heat_addition(3, VL, V[nquad*3-1], P, V, T, work);

    }
};

#endif // CARNOT_H
