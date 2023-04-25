#ifndef OTTO_H
#define OTTO_H

#include <QtCharts>
#include "common.h"
#include "cycle.h"
#include "process.h"

struct Otto: Cycle
{
    Process* p0;
    Process* p1;
    Process* p2;
    Process* p3;

    Otto(): Cycle("Otto")
    {
        p0 = new IsentropicExpansion(np, true);
        p1 = new IsochoricHeating(np);
        p2 = new IsentropicExpansion(np);
        p3 = new IsochoricHeating(np, true);

        processes.push_back(p0);
        processes.push_back(p1);
        processes.push_back(p2);
        processes.push_back(p3);
    }

    void run_processes()
    {
        static_cast<IsentropicExpansion*>(p0)->T2V1V2(TL, VL, VH);
        static_cast<IsochoricHeating*>(p1)->VQT1(VL, qh, p0->T.back());
        static_cast<IsentropicExpansion*>(p2)->T1V1V2(p1->T.back(), VL, VH);
        static_cast<IsochoricHeating*>(p3)->run(VH, TL, p2->T.back());

        heat_input = p1->heat;
    }
};

#endif // OTTO_H
