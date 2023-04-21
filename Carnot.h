#ifndef CARNOT_H
#define CARNOT_H

#include <QtCharts>
#include "common.h"
#include "cycle.h"
#include "process.h"

struct Carnot: Cycle
{
    Process* p0;
    Process* p1;
    Process* p2;
    Process* p3;

    Carnot(): Cycle("Carnot")
    {
        p0 = new IsentropicExpansion(np, true);
        p1 = new IsothermalExpansion(np);
        p2 = new IsentropicExpansion(np, true);
        p3 = new IsothermalExpansion(np, true);

        processes.push_back(p0);
        processes.push_back(p1);
        processes.push_back(p2);
        processes.push_back(p3);
    }

    void run_processes()
    {
        static_cast<IsentropicExpansion*>(p0)->run(VL, TH, TL);
        static_cast<IsentropicExpansion*>(p2)->run(VH, TL, TH);
        static_cast<IsothermalExpansion*>(p1)->run(TH, VL, p2->V.front());
        static_cast<IsothermalExpansion*>(p3)->run(TL, p0->V.front(), VH);

        heat_input = p1->heat;

//        std::cout << "p0 work = " << p0->work << std::endl;
//        std::cout << "p1 work = " << p1->work << std::endl;
//        std::cout << "p2 work = " << p2->work << std::endl;
//        std::cout << "p3 work = " << p3->work << std::endl;
    }
};

#endif // CARNOT_H
