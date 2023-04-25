#ifndef PROCESS_H
#define PROCESS_H

struct Process
{
    std::vector<double> P;
    std::vector<double> V;
    std::vector<double> T;
    std::vector<double> S;
    double work;
    double heat;
    int n;
    double Pmin, Vmin, Tmin, Smin;
    double Pmax, Vmax, Tmax, Smax;

    Process(int n)
    {
        this->n = n;
        P.resize(n);
        V.resize(n);
        T.resize(n);
        S.resize(n);
    }

    void calc_work()
    {
        work = 0;
        for (int j=1; j<n; ++j)
        {
            work += P[j] * (V[j] - V[j-1]);
        }
    }

    void calc_heat()
    {
        heat = 0;
        for (int j=1; j<n; ++j)
        {
            heat += T[j] * (S[j] - S[j-1]);
        }
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
};

struct IsothermalExpansion: Process
{
    bool reverse = false;

    IsothermalExpansion(int n, bool reverse=false): Process(n)
    {
        this->reverse = reverse;
    }

    void run(double Tc, double V1, double V2)
    {
        double inc = (V2 - V1) / (n);

        for (int j=0; j<n; ++j)
        {
            if (reverse)
            {
                V[j] = V2 - j * inc;
            }
            else
            {
                V[j] = V1 + j * inc;
            }
            T[j] = Tc;
            P[j] = m * R * T[j] / V[j];
            S[j] = Sref + cv * std::log(T[j]/Tref) + R * std::log(V[j]/Vref);
        }

        calc_work();
        calc_heat();
        find_min_max();
    }
};

struct IsochoricHeating: Process
{
    bool reverse = false;

    IsochoricHeating(int n, bool reverse=false): Process(n)
    {
        this->reverse = reverse;
    }

    void run(double Vc, double T1, double T2)
    {
        double inc = (T2 - T1) / (n);

        for (int j=0; j<n; ++j)
        {
            if (reverse)
            {
                T[j] = T2 - j * inc;
            }
            else
            {
                T[j] = T1 + j * inc;
            }

            V[j] = Vc;
            P[j] = m * R * T[j] / V[j];
            S[j] = Sref + cv * std::log(T[j]/Tref) + R * std::log(V[j]/Vref);
        }

        calc_work();
        calc_heat();
        find_min_max();
    }

    void VQT1(double Vc, double Q, double T1)
    {
        double T2 = T1 + Q / cv;

        run(Vc, T1, T2);

        double inc = (T2 - T1) / (n);

        for (int j=0; j<n; ++j)
        {
            if (reverse)
            {
                T[j] = T2 - j * inc;
            }
            else
            {
                T[j] = T1 + j * inc;
            }

            V[j] = Vc;
            P[j] = m * R * T[j] / V[j];
            S[j] = Sref + cv * std::log(T[j]/Tref) + R * std::log(V[j]/Vref);
        }

        calc_work();
        calc_heat();
        find_min_max();
    }
};

struct IsentropicExpansion: Process
{
    bool reverse = false;

    IsentropicExpansion(int n, bool reverse=false): Process(n)
    {
        this->reverse = reverse;
    }

    void V1T1T2(double V1, double T1, double T2)
    {
        double inc = (T2 - T1) / (n);

        for (int j=0; j<n; ++j)
        {
            if (reverse)
            {
                T[j] = T2 - j * inc;
            }
            else
            {
                T[j] = T1 + j * inc;
            }

            V[j] = V1 * std::pow(T1/T[j],1/(GM-1));
            P[j] = m * R * T[j] / V[j];
            S[j] = Sref + cv * std::log(T[j]/Tref) + R * std::log(V[j]/Vref);
        }

        calc_work();
        calc_heat();
        find_min_max();
    }

    void T2V1V2(double T2, double V1, double V2)
    {
        double inc = (V2 - V1) / (n);

        for (int j=0; j<n; ++j)
        {
            if (reverse)
            {
                V[j] = V2 - j * inc;
            }
            else
            {
                V[j] = V1 + j * inc;
            }

            T[j] = T2 * std::pow(V2/V[j],(GM-1));
            P[j] = m * R * T[j] / V[j];
            S[j] = Sref + cv * std::log(T[j]/Tref) + R * std::log(V[j]/Vref);
        }

        calc_work();
        calc_heat();
        find_min_max();
    }

    void T1V1V2(double T1, double V1, double V2)
    {
        double inc = (V2 - V1) / (n);

        for (int j=0; j<n; ++j)
        {
            if (reverse)
            {
                V[j] = V2 - j * inc;
            }
            else
            {
                V[j] = V1 + j * inc;
            }

            T[j] = T1 * std::pow(V1/V[j],(GM-1));
            P[j] = m * R * T[j] / V[j];
            S[j] = Sref + cv * std::log(T[j]/Tref) + R * std::log(V[j]/Vref);
        }

        calc_work();
        calc_heat();
        find_min_max();
    }
};




#endif // PROCESS_H
