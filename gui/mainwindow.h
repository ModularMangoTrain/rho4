#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "scopewindow.h"
#include "trajectorywindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QSerialPort *serialPort, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartPWM();
    void onStopPWM();
    void onReadADC();
    void onStartMonitoring();
    void onStopMonitoring();
    void onEnablePID();
    void onDisablePID();
    void onSetFrequency();
    void onSetDuty();
    void onSetSetpoint();
    void onShowScope();
    void onShowTrajectory();
    void readSerialData();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    ScopeWindow *scopeWindow;
    TrajectoryWindow *trajectoryWindow;
    void sendCommand(const QString &cmd);
    void parseAndPlotADC(const QString &line);
    void updateVoltageDisplay(int adcValue);
};
#endif
