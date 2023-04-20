#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

#include "Stirling.h"
#include "Carnot.h"


QChart *chart_pv;
QChart *chart_ts;
//QChartView *chartView_pv;
QValueAxis* x_axis_pv;
QValueAxis* y_axis_pv;
QValueAxis* x_axis_ts;
QValueAxis* y_axis_ts;

Stirling* stirling = nullptr;
Carnot* carnot = nullptr;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    //series = new QLineSeries();
    chart_pv = new QChart();
    chart_ts = new QChart();
    chart_pv->setTitle("PV diagram");
    chart_ts->setTitle("TS diagram");
    x_axis_pv = new QValueAxis();
    y_axis_pv = new QValueAxis();
    x_axis_ts = new QValueAxis();
    y_axis_ts = new QValueAxis();
    x_axis_pv->setTitleText("V");
    y_axis_pv->setTitleText("P");
    x_axis_ts->setTitleText("S");
    y_axis_ts->setTitleText("T");
    chart_pv->addAxis(x_axis_pv, Qt::AlignBottom);
    chart_pv->addAxis(y_axis_pv, Qt::AlignLeft);
    chart_ts->addAxis(x_axis_ts, Qt::AlignBottom);
    chart_ts->addAxis(y_axis_ts, Qt::AlignLeft);
    //chartView = new QChartView(chart);

    //chartView->setRenderHint(QPainter::Antialiasing);


    ui->graphicsView->setChart(chart_pv);
    ui->graphicsView_2->setChart(chart_ts);

    //chart->addSeries(series);
    //stirling->series->attachAxis(x_axis);
    //stirling->series->attachAxis(y_axis);
    //chart->removeSeries(stirling->series);

    ui->doubleSpinBox_cr->setValue(cr);
    ui->lcdNumber_nw_stirling->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber_nw_carnot->setSegmentStyle(QLCDNumber::Flat);

    ui->doubleSpinBox_cr->setMinimum(std::pow((TH / TL), 1 / (GM - 1)));


    ui->spinBox_res->setMaximum(9999999);
    ui->spinBox_res->setValue(n);



    //ui->graphicsView->setChart(chart);




    //GraphicsView->setChart(&chartView);

    //MainWindow w;
    //w.setCentralWidget(chartView);
    //w.show();
    //return a.exec();



    //ui->graphicsView->setChart(chart);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void rescale()
{
    double Pmin, Vmin, Tmin, Smin;
    double Pmax, Vmax, Tmax, Smax;

    Pmax = -1e9;
    Vmax = -1e9;
    Tmax = -1e9;
    Smax = -1e9;

    Pmin = 1e9;
    Vmin = 1e9;
    Tmin = 1e9;
    Smin = 1e9;

    if (stirling != nullptr)
    {
        Pmin = std::min(stirling->Pmin, Pmin);
        Vmin = std::min(stirling->Vmin, Vmin);
        Tmin = std::min(stirling->Tmin, Tmin);
        Smin = std::min(stirling->Smin, Smin);

        Pmax = std::max(stirling->Pmax, Pmax);
        Vmax = std::max(stirling->Vmax, Vmax);
        Tmax = std::max(stirling->Tmax, Tmax);
        Smax = std::max(stirling->Smax, Smax);
    }
    if (carnot != nullptr)
    {
        Pmin = std::min(Pmin, carnot->Pmin);
        Vmin = std::min(Vmin, carnot->Vmin);
        Tmin = std::min(Tmin, carnot->Tmin);
        Smin = std::min(Smin, carnot->Smin);

        Pmax = std::max(Pmax, carnot->Pmax);
        Vmax = std::max(Vmax, carnot->Vmax);
        Tmax = std::max(Tmax, carnot->Tmax);
        Smax = std::max(Smax, carnot->Smax);
    }

    x_axis_pv->setRange(Vmin, Vmax);
    y_axis_pv->setRange(Pmin, Pmax);

    x_axis_ts->setRange(Smin, Smax);
    y_axis_ts->setRange(Tmin, Tmax);

    std::cout << "VL: " << VL << std::endl;
    std::cout << "VH: " << VH << std::endl;

    std::cout << "Vmin: " << Vmin << std::endl;
    std::cout << "Carnot Vmin: " << carnot->Vmin << std::endl;

}



void MainWindow::on_checkBox_stirling_stateChanged(int arg1)
{
    if (ui->checkBox_stirling->isChecked())
    {
        stirling = new Stirling();
        stirling->draw(chart_pv, chart_ts, x_axis_pv, y_axis_pv, x_axis_ts, y_axis_ts);

        ui->lcdNumber_nw_stirling->display(stirling->work);

        rescale();
    }
    else
    {
        chart_pv->removeSeries(stirling->series_pv);
        chart_ts->removeSeries(stirling->series_ts);

        delete stirling;
        stirling = nullptr;

        ui->lcdNumber_nw_stirling->display(0);
    }


}


void MainWindow::on_checkBox_carnot_stateChanged(int arg1)
{
    if (ui->checkBox_carnot->isChecked())
    {
        carnot = new Carnot();
        carnot->draw(chart_pv, chart_ts, x_axis_pv, y_axis_pv, x_axis_ts, y_axis_ts);

        ui->lcdNumber_nw_carnot->display(carnot->work);

        rescale();

        carnot->print("carnot");
    }
    else
    {
        chart_pv->removeSeries(carnot->series_pv);
        chart_ts->removeSeries(carnot->series_ts);

        delete carnot;
        carnot = nullptr;

        ui->lcdNumber_nw_carnot->display(0);
    }


}



void MainWindow::on_doubleSpinBox_cr_valueChanged(double arg1)
{
    cr = ui->doubleSpinBox_cr->value();





    VH = cr * VL;
}



void MainWindow::on_pushButton_cr_clicked()
{
    //chart_pv->removeAllSeries();
    //chart_ts->removeAllSeries();

//    double Pmin, Vmin, Tmin, Smin;
//    double Pmax, Vmax, Tmax, Smax;

//    Pmax = -1e9;
//    Vmax = -1e9;
//    Tmax = -1e9;
//    Smax = -1e9;

//    Pmin = 1e9;
//    Vmin = 1e9;
//    Tmin = 1e9;
//    Smin = 1e9;

    if (stirling != nullptr)
    {
        chart_pv->removeSeries(stirling->series_pv);
        chart_ts->removeSeries(stirling->series_ts);
        stirling->draw(chart_pv, chart_ts, x_axis_pv, y_axis_pv, x_axis_ts, y_axis_ts);

//        Pmin = std::min(stirling->Pmin, Pmin);
//        Vmin = std::min(stirling->Vmin, Vmin);
//        Tmin = std::min(stirling->Tmin, Tmin);
//        Smin = std::min(stirling->Smin, Smin);

//        Pmax = std::max(stirling->Pmax, Pmax);
//        Vmax = std::max(stirling->Vmax, Vmax);
//        Tmax = std::max(stirling->Tmax, Tmax);
//        Smax = std::max(stirling->Smax, Smax);

        ui->lcdNumber_nw_stirling->display(stirling->work);
    }
    if (carnot != nullptr)
    {
        chart_pv->removeSeries(carnot->series_pv);
        chart_ts->removeSeries(carnot->series_ts);
        carnot->draw(chart_pv, chart_ts, x_axis_pv, y_axis_pv, x_axis_ts, y_axis_ts);

//        Pmin = std::min(Pmin, carnot->Pmin);
//        Vmin = std::min(Vmin, carnot->Vmin);
//        Tmin = std::min(Tmin, carnot->Tmin);
//        Smin = std::min(Smin, carnot->Smin);

//        Pmax = std::max(Pmax, carnot->Pmax);
//        Vmax = std::max(Vmax, carnot->Vmax);
//        Tmax = std::max(Tmax, carnot->Tmax);
//        Smax = std::max(Smax, carnot->Smax);

        ui->lcdNumber_nw_carnot->display(carnot->work);
    }

//    x_axis_pv->setRange(Vmin, Vmax);
//    y_axis_pv->setRange(Pmin, Pmax);

//    x_axis_ts->setRange(Smin, Smax);
//    y_axis_ts->setRange(Tmin, Tmax);

    rescale();



}
