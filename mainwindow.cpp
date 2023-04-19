#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

#include "Stirling.h"
#include "Carnot.h"


QChart *chart;
QChartView *chartView;
QValueAxis* x_axis;
QValueAxis* y_axis;

Stirling* stirling = nullptr;
Carnot* carnot = nullptr;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    //series = new QLineSeries();
    chart = new QChart();
    //chart->setTitle("Simple line chart example");
    x_axis = new QValueAxis();
    y_axis = new QValueAxis();
    x_axis->setTitleText("V");
    y_axis->setTitleText("P");
    chart->addAxis(x_axis, Qt::AlignBottom);
    chart->addAxis(y_axis, Qt::AlignLeft);
    chartView = new QChartView(chart);

    chartView->setRenderHint(QPainter::Antialiasing);


    ui->graphicsView->setChart(chart);

    ui->doubleSpinBox_cr->setValue(cr);


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







void MainWindow::on_checkBox_stirling_stateChanged(int arg1)
{
    if (stirling == nullptr)
    {
        stirling = new Stirling();
    }

    if (stirling->first)
    {
        stirling->run();

    }



    if (ui->checkBox_stirling->isChecked())
    {

        stirling->draw(chart);
        stirling->series->attachAxis(x_axis);
        stirling->series->attachAxis(y_axis);


    }
    else
    {
        chart->removeSeries(stirling->series);

    }


}


void MainWindow::on_checkBox_carnot_stateChanged(int arg1)
{
    if (carnot == nullptr)
    {
        carnot = new Carnot();
    }

    if (carnot->first)
    {
        carnot->run();

    }



    if (ui->checkBox_carnot->isChecked())
    {

        carnot->draw(chart);
        carnot->series->attachAxis(x_axis);
        carnot->series->attachAxis(y_axis);


    }
    else
    {
        chart->removeSeries(carnot->series);

    }


}





void MainWindow::on_doubleSpinBox_cr_valueChanged(double arg1)
{
    cr = ui->doubleSpinBox_cr->value();
    VH = cr * VL;
    //std::cout << cr << std::endl;

}


void MainWindow::on_pushButton_cr_clicked()
{
    chart->removeSeries(stirling->series);

    stirling->run();

    stirling->draw(chart);
    std::cout << stirling->Vmin << std::endl;
    std::cout << stirling->Pmin << std::endl;
    x_axis->setRange(stirling->Vmin, stirling->Vmax);
    y_axis->setRange(stirling->Pmin, stirling->Pmax);
        //stirling->series->attachAxis(x_axis);
        //stirling->series->attachAxis(y_axis);
}

