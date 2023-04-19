#include "mainwindow.h"
#include <QApplication>

const int n = 4320;
const double GM = 1.4;
const double cp = 1.004;
const double R = 0.287;
const double TL = 300;
const double TH = 600;
const double VL = 0.1;
const double cr = 6; // compression ratio
const double VH = cr * VL;
const double m = 1;

double P[n];
double V[n];
double T[n];

int nquad = n / 4;
double work = 0.;

double dw(int j)
{
    if (j == 0)
    {
        return 0.;
    }

    return P[j] * (V[j] - V[j-1]);
}

void isothermal_heat_rejection(int k, double Vmin, double Vmax)
{
    double inc = (Vmax - Vmin) / (nquad+1);
    for (int i=0; i<nquad; ++i)
    {
        int j = i + k * nquad;
        //V[j] = VH - i * inc;
        V[j] = Vmax - i * inc;
        T[j] = TL;
        P[j] = m * R * T[j] / V[j];

        work += dw(j);
    }

    //work += m * R * TL * std::log(VI/VH);
}

void isochoric_compression(int k)
{
    double inc = (TH - TL) / (nquad);
    for (int i=0; i<nquad; ++i)
    {
        int j = i + k * nquad;
        V[j] = VL;
        T[j] = TL + i * inc;
        P[j] = m * R * T[j] / V[j];
    }
}

void isothermal_heat_addition(int k, double Vmin, double Vmax)
{
    double inc = (Vmax - Vmin) / (nquad);
    for (int i=0; i<nquad; ++i)
    {
        int j = i + k * nquad;
        //V[j] = VL + i * inc;
        V[j] = Vmin + i * inc;
        T[j] = TH;
        P[j] = m * R * T[j] / V[j];

        work += dw(j);
    }
}

void isochoric_expansion(int k)
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

void stirling()
{
    work = 0.;

    isothermal_heat_rejection(0, VL, VH);
    isochoric_compression(1);
    isothermal_heat_addition(2, VL, VH);
    isochoric_expansion(3);

    //print("stirling");

    //std::cout << "net work from stirling: " << work << std::endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    stirling();




    //GraphicsView->setChart(&chartView);

    MainWindow w;
    //w.setCentralWidget(chartView);
    w.show();
    return a.exec();

}
