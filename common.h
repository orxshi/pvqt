#ifndef COMMON_H
#define COMMON_H

const int n = 4320;
const double GM = 1.4;
const double cp = 1.004;
const double R = 0.287;
const double cv = cp - R;
const double TL = 300;
const double TH = 600;
const double VL = 0.1;
double cr = 6; // compression ratio
double VH = cr * VL;
const double m = 1;

int nquad = n / 4;

double dw(int j, double P[n], double V[n], double T[n])
{
    if (j == 0)
    {
        return 0.;
    }

    return P[j] * (V[j] - V[j-1]);
}

double net_work(double P[n], double V[n])
{
    double nw = 0;
    for (int j=1; j<n; ++j)
    {
        nw += P[j] * (V[j] - V[j-1]);
    }
    return nw;
}

void isothermal_heat_rejection(int k, double Vmin, double Vmax, double P[n], double V[n], double T[n], double& work)
{
    double inc = (Vmax - Vmin) / (nquad);
    assert(Vmax > Vmin);
    if (Vmax < Vmin)
    {
        std::cout << "aaaaaaaaaaaaaaaaaaaaaaabbbbbbb" << std::endl;
    }
    for (int i=0; i<nquad; ++i)
    {
        int j = i + k * nquad;
        //V[j] = VH - i * inc;
        V[j] = Vmax - i * inc;
        T[j] = TL;
        P[j] = m * R * T[j] / V[j];

        //work += dw(j, P, V, T);
    }

    //work += m * R * TL * std::log(VI/VH);


}

void isochoric_compression(int k, double P[n], double V[n], double T[n])
{
    double inc = (TH - TL) / (nquad);
    for (int i=0; i<nquad; ++i)
    {
        int j = i + k * nquad;
        V[j] = VL;
        T[j] = TL + i * inc;
        P[j] = m * R * T[j] / V[j];

        //std::cout << TL << " " << T[j] << " " << i << " " << inc << std::endl;
    }
}

void isothermal_heat_addition(int k, double Vmin, double Vmax, double P[n], double V[n], double T[n], double& work)
{
    double inc = (Vmax - Vmin) / (nquad);
    assert(Vmax > Vmin);
    for (int i=0; i<nquad; ++i)
    {
        int j = i + k * nquad;
        //V[j] = VL + i * inc;
        V[j] = Vmin + i * inc;
        T[j] = TH;
        P[j] = m * R * T[j] / V[j];

        //work += dw(j, P, V, T);
    }

    std::cout << V[0+k*nquad] << std::endl;
}

void isochoric_expansion(int k, double P[n], double V[n], double T[n])
{
    double inc = (TH - TL) / (nquad);
    for (int i=0; i<nquad; ++i)
    {
        int j = i + k * nquad;
        V[j] = VH;
        T[j] = TH - i * inc;
        P[j] = m * R * T[j] / V[j];
    }
}

void isentropic_compression(int k, double P[n], double V[n], double T[n], double& work)
{
    double inct = (TH - TL) / (nquad);
    //double VI = VL * std::pow(TH/TL,1/(GM-1));
    //double incv = (VI - VL) / (nquad);
    for (int i=0; i<nquad; ++i)
    {
        int j = i + k * nquad;
        //V[j] = VI - i * incv;
        T[j] = TH - i * inct;
        V[j] = VL * std::pow(TH/T[j],1/(GM-1));
        P[j] = m * R * T[j] / V[j];

        //work += dw(j, P, V, T);
    }

    std::cout << V[nquad-1] << std::endl;
}

void isentropic_expansion(int k, double P[n], double V[n], double T[n], double& work)
{
    double inct = (TH - TL) / (nquad);
    //double VI = VH * std::pow(TL/TH,1/(GM-1));
    //double incv = (VH - VI) / (nquad);
    //assert(VI < VH);
    for (int i=0; i<nquad; ++i)
    {
        int j = i + k * nquad;
        //V[j] = VH - i * incv;
        T[j] = TL + i * inct;
        V[j] = VH * std::pow(TL/T[j],1/(GM-1));
        P[j] = m * R * T[j] / V[j];

        //work += dw(j, P, V, T);
    }
}

void get_entropy(double V[n], double T[n], double S[n])
{
    S[0] = 0;
    for (int j=1; j<n; ++j)
    {
        S[j] = S[j-1] + cv * std::log(T[j]/T[j-1]) + R * std::log(V[j]/V[j-1]);
        //std::cout << T[j] << std::endl;
    }
}

#endif // COMMON_H
