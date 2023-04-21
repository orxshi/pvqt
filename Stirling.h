#ifndef STIRLING_H
#define STIRLING_H

#include <QtCharts>
#include "common.h"
#include "cycle.h"
#include "process.h"

struct Stirling: Cycle
{
    Process* p0;
    Process* p1;
    Process* p2;
    Process* p3;

    Stirling(): Cycle("Stirling")
    {
        p0 = new IsothermalExpansion(np, true);
        p1 = new IsochoricHeating(np);
        p2 = new IsothermalExpansion(np);
        p3 = new IsochoricHeating(np, true);

        processes.push_back(p0);
        processes.push_back(p1);
        processes.push_back(p2);
        processes.push_back(p3);
    }

    void run_processes()
    {
        static_cast<IsothermalExpansion*>(p0)->run(TL, VL, VH);
        static_cast<IsochoricHeating*>(p1)->run(VL, TL, TH);
        static_cast<IsothermalExpansion*>(p2)->run(TH, VL, VH);
        static_cast<IsochoricHeating*>(p3)->run(VH, TL, TH);

        heat_input = p2->heat;
    }
};

#endif // STIRLING_H
