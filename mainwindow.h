#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_checkBox_carnot_stateChanged(int arg1);
    void on_checkBox_stirling_stateChanged(int arg1);
    void on_checkBox_otto_stateChanged(int arg1);




    void on_doubleSpinBox_cr_valueChanged(double arg1);

    void on_pushButton_cr_clicked();


    void on_doubleSpinBox_extemp_valueChanged(double arg1);

    void on_doubleSpinBox_comtemp_valueChanged(double arg1);

    void on_doubleSpinBox_qh_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
