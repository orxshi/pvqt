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
        static_cast<IsentropicExpansion*>(p0)->V1T1T2(VL, TH, TL);
        static_cast<IsentropicExpansion*>(p2)->V1T1T2(VH, TL, TH);
        static_cast<IsothermalExpansion*>(p1)->run(TH, VL, p2->V.front());
        static_cast<IsothermalExpansion*>(p3)->run(TL, p0->V.front(), VH);

        heat_input = p1->heat;

        for (int i=0; i<p3->n; ++i)
        {
            series_pv_ql->append(p3->V[i], p3->P[i]);
            series_pv_ql_lower->append(p3->V[i], 0);
        }


        //series_pv_ql_lower->append(VL, 0);
        //series_pv_ql_lower->append(VH, 0);






//        std::cout << "p0 work = " << p0->work << std::endl;
//        std::cout << "p1 work = " << p1->work << std::endl;
//        std::cout << "p2 work = " << p2->work << std::endl;
//        std::cout << "p3 work = " << p3->work << std::endl;
    }
};

#endif // CARNOT_H
