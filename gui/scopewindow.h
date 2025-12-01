#ifndef SCOPEWINDOW_H
#define SCOPEWINDOW_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

class ScopeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ScopeWindow(QWidget *parent = nullptr);
    ~ScopeWindow();

public slots:
    void addDataPoint(int adcValue);
    void clearData();

private:
    QLineSeries *adcSeries;
    QChart *chart;
    QValueAxis *axisX;
    QValueAxis *axisY;
    qreal timeCounter;
};

#endif
