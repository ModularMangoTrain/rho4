#include "scopewindow.h"
#include <QVBoxLayout>

ScopeWindow::ScopeWindow(QWidget *parent)
    : QWidget(parent)
    , timeCounter(0)
{
    setWindowTitle("ADC Oscilloscope");
    resize(800, 400);
    
    adcSeries = new QLineSeries();
    chart = new QChart();
    chart->addSeries(adcSeries);
    chart->setTitle("ADC Monitor");
    chart->legend()->hide();
    
    axisX = new QValueAxis();
    axisX->setTitleText("Time (s)");
    axisX->setRange(0, 10);
    chart->addAxis(axisX, Qt::AlignBottom);
    adcSeries->attachAxis(axisX);
    
    axisY = new QValueAxis();
    axisY->setTitleText("ADC Value");
    axisY->setRange(0, 1023);
    chart->addAxis(axisY, Qt::AlignLeft);
    adcSeries->attachAxis(axisY);
    
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
}

ScopeWindow::~ScopeWindow()
{
}

void ScopeWindow::addDataPoint(int adcValue)
{
    adcSeries->append(timeCounter, adcValue);
    timeCounter += 0.1;
    
    if (timeCounter > 10) {
        axisX->setRange(timeCounter - 10, timeCounter);
    }
    
    if (adcSeries->count() > 200) {
        adcSeries->remove(0);
    }
}

void ScopeWindow::clearData()
{
    adcSeries->clear();
    timeCounter = 0;
    axisX->setRange(0, 10);
}
