#ifndef CYCLE_H
#define CYCLE_H

#include <fstream>

struct Cycle
{
    bool first;
    double Pmin, Vmin, Tmin, Smin;
    double Pmax, Vmax, Tmax, Smax;
    double P[n];
    double V[n];
    double T[n];
    double S[n];
    double work;

    QLineSeries *series_pv;
    QLineSeries *series_ts;

    Cycle()
    {
        series_pv = new QLineSeries();
        series_ts = new QLineSeries();

        first = true;
    }

    ~Cycle()
    {
        delete series_pv;
        delete series_ts;
    }

    void find_min_max()
    {
        Pmax = -1e9;
        Vmax = -1e9;
        Tmax = -1e9;
        Smax = -1e9;
        for (int i=0; i<n; ++i)
        {
            Pmax = std::max(Pmax, P[i]);
            Vmax = std::max(Vmax, V[i]);
            Tmax = std::max(Tmax, T[i]);
            Smax = std::max(Smax, S[i]);
        }

        Pmin = 1e9;
        Vmin = 1e9;
        Tmin = 1e9;
        Smin = 1e9;
        for (int i=0; i<n; ++i)
        {
            Pmin = std::min(Pmin, P[i]);
            Vmin = std::min(Vmin, V[i]);
            Tmin = std::min(Tmin, T[i]);
            Smin = std::min(Smin, S[i]);
        }
    }

    virtual void processes() = 0;

    void run()
    {
        work = 0.;

        for (int i=0; i<n; ++i)
        {
            P[i] = 10;
            V[i] = 10;
            T[i] = 10;
            S[i] = 10;
        }

        processes();

        get_entropy(V, T, S);

        find_min_max();

        first = false;

        series_pv->clear();
        series_ts->clear();
        for (int i=0; i<n; ++i)
        {
            series_pv->append(V[i], P[i]);
            series_ts->append(S[i], T[i]);
        }

        work = net_work(P, V);
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
        //if (first)
        //{
            run();
        //}



        draw_pv(chart_pv);
        series_pv->attachAxis(x_axis_pv);
        series_pv->attachAxis(y_axis_pv);

        draw_ts(chart_ts);
        series_ts->attachAxis(x_axis_ts);
        series_ts->attachAxis(y_axis_ts);
    }

    void print(std::string filename)
    {
        std::cout << "aaaa" << std::endl;
        std::ofstream out;
        out.open(filename);

        for (int i=0; i<n; ++i)
        {
            out << i;
            out << " ";
            out << P[i];
            out << " ";
            out << V[i];
            out << " ";
            out << T[i];
            out << " ";
            out << S[i];
            out << std::endl;
        }

        out.close();
    }
};

#endif // CYCLE_H
