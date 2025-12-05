#include "usrScopeWindow.h"
#include <QVBoxLayout>
#include <QMouseEvent>
#include <algorithm>

// essentially reading the user input of each continuous stroke and making it into a feasible waveform
// i.e. not just scribbling but adjusting a predefined line to different amplitudes => if you back track then
// it changes the previous amplitude made to the new one rather than draw on top of it

DrawChartView::DrawChartView(QChart *chart, UsrScopeWindow *owner)
    : QChartView(chart), owner(owner)
{
    setMouseTracking(true);
}

void DrawChartView::mousePressEvent(QMouseEvent *ev)
{
    if (ev->buttons() & Qt::LeftButton)
        owner->handleMousePress(ev->pos());
    QChartView::mousePressEvent(ev);
}

void DrawChartView::mouseMoveEvent(QMouseEvent *ev)
{
    if (ev->buttons() & Qt::LeftButton)
        owner->handleMouseMove(ev->pos());
    QChartView::mouseMoveEvent(ev);
}

void DrawChartView::mouseReleaseEvent(QMouseEvent *ev)
{
    owner->handleMouseRelease(ev->pos());
    QChartView::mouseReleaseEvent(ev);
}

// these just make it so that so long as the user is holding the mouse then changes ccan be made to the graph

UsrScopeWindow::UsrScopeWindow(QWidget *parent)
    : QWidget(parent),
    playTimer(new QTimer(this)),
    playIndex(0),
    sampleIntervalMs(200),
    looping(true),
    lastIndex(-1)
{
    setWindowTitle("User Oscilloscope");

    adcSeries = new QLineSeries();
    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(adcSeries);
    chart->setTitle("User Monitor");

    setMinimumSize(800,600);

    axisX = new QValueAxis();
    axisX->setTitleText("Time (s)");
    axisX->setRange(0, 10);
    chart->addAxis(axisX, Qt::AlignBottom);
    adcSeries->attachAxis(axisX);

    axisY = new QValueAxis();
    axisY->setTitleText("User ADC Value");
    axisY->setRange(0, 1023);
    chart->addAxis(axisY, Qt::AlignLeft);
    adcSeries->attachAxis(axisY);

    chartView = new DrawChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);

    connect(playTimer, &QTimer::timeout, this, &UsrScopeWindow::onPlayTimer);
}

void UsrScopeWindow::clearData()
{
    samples.clear();
    adcSeries->clear();
    lastIndex = -1;
}

int UsrScopeWindow::yToSetpoint(int y) const
{
    int h = chartView->height();
    if (h <= 1) return 512;
    int v = (h - 1 - y);
    //clamp v between 0 and widget height - 1
    v = std::clamp(v, 0, h - 1);

    return (v * 1023) / (h - 1);
}

int UsrScopeWindow::xToIndex(int x) const
{
    if (samples.isEmpty()) return 0;
    int idx = x * (samples.size() - 1) / std::max(1, chartView->width() - 1);

    //clamp index between 0 and widget width - 1
    return std::clamp(idx, 0, static_cast<int>(samples.size() - 1)); // had to static cast to stop conflicting qsize and int for clamp to work
}

void UsrScopeWindow::handleMousePress(const QPoint &pos)
{
    if (samples.isEmpty())
        samples.fill(512, chartView->width());

    lastIndex = -1;
    handleMouseMove(pos);
}

void UsrScopeWindow::handleMouseMove(const QPoint &pos)
{
    if (samples.isEmpty())
        samples.fill(512, chartView->width());

    int idx = xToIndex(pos.x());
    int sp  = yToSetpoint(pos.y());

    if (lastIndex < 0) lastIndex = idx;

    int a = std::min(lastIndex, idx);
    int b = std::max(lastIndex, idx);

    for (int i = a; i <= b; i++)
    {
        float t = (b == a ? 1.f : float(i - a) / float(b - a));
        samples[i] = samples[lastIndex] + t * (sp - samples[lastIndex]);
    }

    lastIndex = idx;
    updateSeries();
}

void UsrScopeWindow::handleMouseRelease(const QPoint &)
{
    lastIndex = -1;
}

void UsrScopeWindow::resizeEvent(QResizeEvent *ev)
{
    QWidget::resizeEvent(ev);

    int newLen = chartView->width();
    if (newLen <= 0) return;

    if (samples.isEmpty())
    {
        samples.fill(512, newLen);
    }
    else if (samples.size() != newLen)
    {
        QVector<int> newSamples(newLen);
        for (int i = 0; i < newLen; ++i)
        {
            int oldIndex = i * (samples.size() - 1) / (newLen - 1);
            newSamples[i] = samples[oldIndex];
        }
        samples = std::move(newSamples);
        updateSeries();
    }
}

void UsrScopeWindow::updateSeries()
{
    if (samples.isEmpty()) return;

    QVector<QPointF> pts;
    pts.reserve(samples.size());

    const float dx = 10.0f / std::max(1, static_cast<int>(samples.size() - 1)); // once again had to force to be an int rather than qsize :(
    for (int i = 0; i < samples.size(); ++i)
        pts.append(QPointF(i * dx, samples[i]));

    adcSeries->replace(pts);
}

void UsrScopeWindow::startPlayback(int intervalMs, bool loop)
{
    if (samples.isEmpty()) return;
    sampleIntervalMs = intervalMs;
    looping = loop;
    playIndex = 0;
    playTimer->start(sampleIntervalMs);
}

void UsrScopeWindow::stopPlayback()
{
    playTimer->stop();
}

void UsrScopeWindow::onPlayTimer()
{
    if (samples.isEmpty())
    {
        playTimer->stop();
        return;
    }

    emit playbackSetpoint(samples[playIndex]);

    if (++playIndex >= samples.size())
    {
        if (looping)
            playIndex = 0;
        else
            playTimer->stop();
    }
}
