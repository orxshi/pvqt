#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

#include "Stirling.h"
#include "Carnot.h"
#include "Otto.h"


QChart *chart_pv;
QChart *chart_ts;
//QChartView *chartView_pv;
QValueAxis* x_axis_pv;
QValueAxis* y_axis_pv;
QValueAxis* x_axis_ts;
QValueAxis* y_axis_ts;

Stirling* stirling = nullptr;
Carnot* carnot = nullptr;
Otto* otto = nullptr;


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
    //chart_pv->legend()->setAlignment(Qt::AlignBottom);
    //chart_ts->legend()->setAlignment(Qt::AlignBottom);
    //chartView = new QChartView(chart);

    //chartView->setRenderHint(QPainter::Antialiasing);


    ui->graphicsView->setChart(chart_pv);
    ui->graphicsView_2->setChart(chart_ts);
    //ui->graphicsView->setMinimumSize(10, 10);



    //chart->addSeries(series);
    //stirling->series->attachAxis(x_axis);
    //stirling->series->attachAxis(y_axis);
    //chart->removeSeries(stirling->series);

    ui->doubleSpinBox_cr->setValue(cr);
    ui->doubleSpinBox_extemp->setValue(TL);
    ui->doubleSpinBox_comtemp->setValue(TH);
    ui->lcdNumber_nw_stirling->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber_nw_carnot->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber_nw_carnot->setDigitCount(4);
    ui->lcdNumber_nw_stirling->setDigitCount(4);
    ui->lcdNumber_eff_carnot->setDigitCount(4);
    ui->lcdNumber_eff_stirling->setDigitCount(4);

    ui->doubleSpinBox_qh->setValue(qh);


    ui->doubleSpinBox_cr->setMinimum(std::pow((TH / TL), 1 / (GM - 1)));
    //ui->doubleSpinBox_comtemp->setMaximum(std::pow(cr, GM - 1) * TL);

//    ui->tableWidget_cycle->setRowCount(10);
//    ui->tableWidget_cycle->setColumnCount(3);

//    QTableWidgetItem* header0 = new QTableWidgetItem();
//    QTableWidgetItem* header1 = new QTableWidgetItem();
//    QTableWidgetItem* header2 = new QTableWidgetItem();

//    header0->setText("Cycle");
//    ui->tableWidget_cycle->setHorizontalHeaderItem(0,header0);

//    header1->setText("Power (kW)");
//    ui->tableWidget_cycle->setHorizontalHeaderItem(1,header1);

//    header2->setText("Efficiency (%)");
//    ui->tableWidget_cycle->setHorizontalHeaderItem(2,header2);

//    ui->tableWidget_cycle->verticalHeader()->setVisible(false);
//    ui->tableWidget_cycle->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    //ui->spinBox_res->setMaximum(9999999);
    //ui->spinBox_res->setValue(n);



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



void rescale(bool for_shading=false)
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
    if (otto != nullptr)
    {
        Pmin = std::min(Pmin, otto->Pmin);
        Vmin = std::min(Vmin, otto->Vmin);
        Tmin = std::min(Tmin, otto->Tmin);
        Smin = std::min(Smin, otto->Smin);

        Pmax = std::max(Pmax, otto->Pmax);
        Vmax = std::max(Vmax, otto->Vmax);
        Tmax = std::max(Tmax, otto->Tmax);
        Smax = std::max(Smax, otto->Smax);
    }

    if (for_shading)
    {
        Pmin = 0;
        Tmin = 0;
    }

    x_axis_pv->setRange(Vmin, Vmax);
    y_axis_pv->setRange(Pmin, Pmax);

    x_axis_ts->setRange(Smin, Smax);
    y_axis_ts->setRange(Tmin, Tmax);
}



void MainWindow::on_checkBox_stirling_stateChanged(int arg1)
{
    if (ui->checkBox_stirling->isChecked())
    {
        stirling = new Stirling();
        stirling->draw(chart_pv, chart_ts, x_axis_pv, y_axis_pv, x_axis_ts, y_axis_ts);

        ui->lcdNumber_nw_stirling->display(stirling->work);
        ui->lcdNumber_eff_stirling->display(stirling->eff);

        rescale();
    }
    else
    {
        chart_pv->removeSeries(stirling->series_pv);
        chart_ts->removeSeries(stirling->series_ts);

        delete stirling;
        stirling = nullptr;

        ui->lcdNumber_nw_stirling->display(0);
        ui->lcdNumber_eff_stirling->display(0);
    }


}


void MainWindow::on_checkBox_carnot_stateChanged(int arg1)
{
    if (ui->checkBox_carnot->isChecked())
    {
        carnot = new Carnot();
        carnot->draw(chart_pv, chart_ts, x_axis_pv, y_axis_pv, x_axis_ts, y_axis_ts);

        ui->lcdNumber_nw_carnot->display(carnot->work);
        ui->lcdNumber_eff_carnot->display(carnot->eff);
        ui->doubleSpinBox_qh->setValue(carnot->heat_input);


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
        ui->lcdNumber_eff_carnot->display(0);
        ui->doubleSpinBox_qh->setValue(0);

    }


}

void MainWindow::on_checkBox_otto_stateChanged(int arg1)
{
    if (ui->checkBox_otto->isChecked())
    {
        otto = new Otto();
        otto->draw(chart_pv, chart_ts, x_axis_pv, y_axis_pv, x_axis_ts, y_axis_ts);

        ui->lcdNumber_nw_otto->display(otto->work);
        ui->lcdNumber_eff_otto->display(otto->eff);
        //ui->doubleSpinBox_qh->setValue(otto->heat_input);
        ui->doubleSpinBox_comtemp->setValue(otto->p1->T.back());



        rescale();

        otto->print("otto");
    }
    else
    {
        chart_pv->removeSeries(otto->series_pv);
        chart_ts->removeSeries(otto->series_ts);

        delete otto;
        otto = nullptr;

        ui->lcdNumber_nw_otto->display(0);
        ui->lcdNumber_eff_otto->display(0);
        //ui->doubleSpinBox_qh->setValue(0);
        ui->doubleSpinBox_comtemp->setValue(0);


    }


}


void MainWindow::on_doubleSpinBox_cr_valueChanged(double arg1)
{
    cr = ui->doubleSpinBox_cr->value();

    //ui->doubleSpinBox_comtemp->setMaximum(std::pow(GM, GM - 1) * TL);

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
        ui->lcdNumber_eff_stirling->display(stirling->eff);

    }
    if (carnot != nullptr)
    {        
        chart_pv->removeSeries(carnot->series_pv);
        chart_ts->removeSeries(carnot->series_ts);
        chart_pv->removeSeries(carnot->series_pv_ql_area);
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
        ui->lcdNumber_eff_carnot->display(carnot->eff);
        ui->doubleSpinBox_qh->setValue(carnot->heat_input);


        //chart_pv->addSeries(carnot->series_pv_ql_area);

    }
    if (otto != nullptr)
    {
        chart_pv->removeSeries(otto->series_pv);
        chart_ts->removeSeries(otto->series_ts);
        otto->draw(chart_pv, chart_ts, x_axis_pv, y_axis_pv, x_axis_ts, y_axis_ts);

        ui->lcdNumber_nw_otto->display(otto->work);
        ui->lcdNumber_eff_otto->display(otto->eff);
        ui->doubleSpinBox_comtemp->setValue(otto->p1->T.back());
    }

//    x_axis_pv->setRange(Vmin, Vmax);
//    y_axis_pv->setRange(Pmin, Pmax);

//    x_axis_ts->setRange(Smin, Smax);
//    y_axis_ts->setRange(Tmin, Tmax);

    rescale();



}

//void MainWindow::on_actionCarnot_triggered()
//{
//    carnot = new Carnot();
//    carnot->draw(chart_pv, chart_ts, x_axis_pv, y_axis_pv, x_axis_ts, y_axis_ts);

//    ui->lcdNumber_nw_carnot->display(carnot->work);
//    ui->lcdNumber_eff_carnot->display(carnot->eff);


//    rescale();

//    //carnot->print("carnot");



//    QTableWidgetItem *item = new QTableWidgetItem();
//    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
//    item->setText("Carnot");
//    ui->tableWidget_cycle->setItem(0, 0, item);

//    QTableWidgetItem *item1 = new QTableWidgetItem();
//    item1->setFlags(item1->flags() ^ Qt::ItemIsEditable);
//    item1->setText(QString::number(carnot->work, 'f', 2));
//    ui->tableWidget_cycle->setItem(0, 1, item1);

//    QTableWidgetItem *item2 = new QTableWidgetItem();
//    item2->setFlags(item2->flags() ^ Qt::ItemIsEditable);
//    item2->setText(QString::number(carnot->eff * 100, 'f', 2));
//    ui->tableWidget_cycle->setItem(0, 2, item2);
//}


void MainWindow::on_doubleSpinBox_extemp_valueChanged(double arg1)
{
    TL = ui->doubleSpinBox_extemp->value();

    ui->doubleSpinBox_cr->setMinimum(std::pow((TH / TL), 1 / (GM - 1)));
    //ui->doubleSpinBox_comtemp->setMaximum(std::pow(cr, GM - 1) * TL);
}


void MainWindow::on_doubleSpinBox_comtemp_valueChanged(double arg1)
{
    TH = ui->doubleSpinBox_comtemp->value();
}


void MainWindow::on_doubleSpinBox_qh_valueChanged(double arg1)
{
    qh = ui->doubleSpinBox_qh->value();
}

