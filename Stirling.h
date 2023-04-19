#ifndef STIRLING_H
#define STIRLING_H

#include <QtCharts>
#include "common.h"







struct Stirling
{
    bool first;
    double Pmin, Vmin, Tmin;
    double Pmax, Vmax, Tmax;

    QLineSeries *series;
    //QChart *chart;
    //QChartView *chartView;

    Stirling()
    {
        series = new QLineSeries();
        series->setName("Stirling");
        //chart = new QChart();
        //chartView = new QChartView(chart);
        first = true;
    }

    double P[n];
    double V[n];
    double T[n];
    double work;

    void run()
    {

        work = 0.;

        isothermal_heat_rejection(0, VL, VH, P, V, T, work);
        isochoric_compression(1, P, V, T);
        isothermal_heat_addition(2, VL, VH, P, V, T, work);
        isochoric_expansion(3, P, V, T);

        Pmax = -1e9;
        Vmax = -1e9;
        Tmax = -1e9;
        for (int i=0; i<n; ++i)
        {
            Pmax = std::max(Pmax, P[i]);
            Vmax = std::max(Vmax, V[i]);
            Tmax = std::max(Tmax, T[i]);
        }

        Pmin = 1e9;
        Vmin = 1e9;
        Tmin = 1e9;
        for (int i=0; i<n; ++i)
        {
            Pmin = std::min(Pmin, P[i]);
            Vmin = std::min(Vmin, V[i]);
            Tmin = std::min(Tmin, T[i]);
        }

        first = false;

        series->clear();
        for (int i=0; i<n; ++i)
        {
            series->append(V[i], P[i]);
        }

    }



    void draw(QChart* chart)
    {

        chart->addSeries(series);
    }

};

#endif // STIRLING_H
