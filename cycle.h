#ifndef CYCLE_H
#define CYCLE_H

#include <fstream>
#include "process.h"

struct Cycle
{
    double Pmin, Vmin, Tmin, Smin;
    double Pmax, Vmax, Tmax, Smax;
    std::vector<Process*> processes;
    double work;
    double heat_input;
    double eff;

    QLineSeries *series_pv;
    QLineSeries *series_ts;
    QLineSeries *series_pv_ql;
    QLineSeries *series_pv_ql_lower;
    QAreaSeries* series_pv_ql_area;

    Cycle(QString name)
    {
        series_pv = new QLineSeries();
        series_ts = new QLineSeries();
        series_pv_ql = new QLineSeries();
        series_pv_ql_lower = new QLineSeries();
        series_pv_ql_area = new QAreaSeries();



        series_pv_ql_area->setUpperSeries(series_pv_ql);
        series_pv_ql_area->setLowerSeries(series_pv_ql_lower);


        series_pv->setName(name);
        series_ts->setName(name);
    }

    ~Cycle()
    {
        delete series_pv;
        delete series_ts;
        delete series_pv_ql;
        delete series_pv_ql_lower;
        delete series_pv_ql_area;
    }

    void find_min_max()
    {
        Pmax = -1e9;
        Vmax = -1e9;
        Tmax = -1e9;
        Smax = -1e9;
        for (Process* process: processes)
        {
            Pmax = std::max(Pmax, process->Pmax);
            Vmax = std::max(Vmax, process->Vmax);
            Tmax = std::max(Tmax, process->Tmax);
            Smax = std::max(Smax, process->Smax);

        }

        Pmin = 1e9;
        Vmin = 1e9;
        Tmin = 1e9;
        Smin = 1e9;
        for (Process* process: processes)
        {
            Pmin = std::min(Pmin, process->Pmin);
            Vmin = std::min(Vmin, process->Vmin);
            Tmin = std::min(Tmin, process->Tmin);
            Smin = std::min(Smin, process->Smin);
        }
    }

    virtual void run_processes() = 0;

    void run()
    {
        series_pv->clear();
        series_ts->clear();
        series_pv_ql->clear();
        series_pv_ql_lower->clear();

        run_processes();

        find_min_max();



        for (Process* process: processes)
        {
            for (int i=0; i<process->n; ++i)
            {
                series_pv->append(process->V[i], process->P[i]);
                series_ts->append(process->S[i], process->T[i]);
            }
        }

        work = 0.;
        for (Process* process: processes)
        {
            work += process->work;
        }

        eff = work / heat_input * 100;


    }

    void draw_pv(QChart* chart_pv)
    {
        chart_pv->addSeries(series_pv);
    }

    void draw_ts(QChart* chart_ts)
    {
        chart_ts->addSeries(series_ts);
    }

    void draw(QChart* chart_pv, QChart* chart_ts, QValueAxis* x_axis_pv, QValueAxis* y_axis_pv, QValueAxis* x_axis_ts, QValueAxis* y_axis_ts)
    {        
        run();

        draw_pv(chart_pv);
        series_pv->attachAxis(x_axis_pv);
        series_pv->attachAxis(y_axis_pv);

        draw_ts(chart_ts);
        series_ts->attachAxis(x_axis_ts);
        series_ts->attachAxis(y_axis_ts);
    }

    void print(std::string filename)
    {
        std::ofstream out;
        out.open(filename);

        for (Process* process: processes)
        {
            for (int i=0; i<process->n; ++i)
            {
                //out << i;
                //out << " ";
                out << process->P[i];
                out << " ";
                out << process->V[i];
                out << " ";
                out << process->T[i];
                out << " ";
                out << process->S[i];
                out << std::endl;
            }

        }

        out.close();
    }
};

#endif // CYCLE_H
