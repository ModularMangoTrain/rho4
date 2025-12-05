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
    
    QPen pen(QColor(0, 188, 212));
    pen.setWidth(3);
    adcSeries->setPen(pen);
    chart->setTitle("ADC Monitor");
    chart->setTitleBrush(QBrush(Qt::white));
    chart->legend()->hide();
    chart->setBackgroundBrush(QBrush(QColor(25, 35, 45)));
    chart->setPlotAreaBackgroundBrush(QBrush(QColor(35, 45, 55)));
    chart->setPlotAreaBackgroundVisible(true);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setAnimationOptions(QChart::NoAnimation);
    
    axisX = new QValueAxis();
    axisX->setTitleText("Time (s)");
    axisX->setTitleBrush(QBrush(Qt::white));
    axisX->setLabelsBrush(QBrush(Qt::white));
    axisX->setRange(0, 10);
    axisX->setGridLineColor(QColor(60, 70, 80));
    chart->addAxis(axisX, Qt::AlignBottom);
    adcSeries->attachAxis(axisX);
    
    axisY = new QValueAxis();
    axisY->setTitleText("ADC Value");
    axisY->setTitleBrush(QBrush(Qt::white));
    axisY->setLabelsBrush(QBrush(Qt::white));
    axisY->setRange(0, 1023);
    axisY->setGridLineColor(QColor(60, 70, 80));
    chart->addAxis(axisY, Qt::AlignLeft);
    adcSeries->attachAxis(axisY);
    
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRenderHint(QPainter::SmoothPixmapTransform);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(chartView);
    
    setStyleSheet("background-color: #192330;");
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
