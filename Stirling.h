#ifndef STIRLING_H
#define STIRLING_H

#include <QtCharts>
#include "common.h"
#include "cycle.h"

struct Stirling: Cycle
{
    Stirling()
    {        
        series_pv->setName("Stirling");
        series_ts->setName("Stirling");
    }   

    void processes()
    {
        isothermal_heat_rejection(0, VL, VH, P, V, T, work);
        isochoric_compression(1, P, V, T);
        isothermal_heat_addition(2, VL, VH, P, V, T, work);
        isochoric_expansion(3, P, V, T);
    }
};

#endif // STIRLING_H
