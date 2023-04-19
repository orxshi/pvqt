#ifndef CARNOT_H
#define CARNOT_H

#include <QtCharts>
#include "common.h"




struct Carnot
{
    bool first;

    QLineSeries *series;

    Carnot()
    {
        series = new QLineSeries();
        first = true;
    }

    double P[n];
    double V[n];
    double T[n];
    double work;

    void run()
    {
        work = 0.;

        isentropic_compression(0, P, V, T, work);
        isentropic_expansion(2, P, V, T, work);
        isothermal_heat_addition(1, VL, V[nquad*2], P, V, T, work);
        isothermal_heat_rejection(3, V[0], VH, P, V, T, work);



        first = false;

        for (int i=0; i<n; ++i)
        {
            series->append(V[i], P[i]);
        }
    }

    void draw(QChart* chart)
    {
        series->setName("Carnot");
        chart->addSeries(series);

    }
};

#endif // CARNOT_H
