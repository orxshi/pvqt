#include "mainwindow.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //GraphicsView->setChart(&chartView);

    MainWindow w;
    //w.setCentralWidget(chartView);
    w.show();
    return a.exec();

}
