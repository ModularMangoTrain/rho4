#ifndef USRSCOPEWINDOW_H
#define USRSCOPEWINDOW_H

#include <QWidget>
#include <QVector>
#include <QTimer>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QResizeEvent>

class UsrScopeWindow : public QWidget
{
    Q_OBJECT
public:
    explicit UsrScopeWindow(QWidget *parent = nullptr);
    QSize minimumSizeHint() const override { return QSize(400, 200); }

public slots:
    void clearData();
    void startPlayback(int intervalMs, bool loop = true);
    void stopPlayback();
    void setSampleRateMs(int ms) {
        sampleIntervalMs = ms;
        if (playTimer) playTimer->setInterval(ms);
    }

signals:
    void playbackSetpoint(int setpoint); // 0 => 1023

protected:
    //allows for overlay of drawing and chart
    friend class DrawChartView;

    // should allow user to draw out their continuous waveform and
    // then the PID should set the ADC value approx and fingers crossed
    // the waveform will look the same as the drawing
    void handleMousePress(const QPoint &pos);
    void handleMouseMove(const QPoint &pos);
    void handleMouseRelease(const QPoint &pos);
    void resizeEvent(QResizeEvent *ev) override;

    int yToSetpoint(int y) const;
    int xToIndex(int x) const;
    void updateSeries();

private slots:
    void onPlayTimer();

private:
    QVector<int> samples; // stored as ADC-scale 0 => 1023
    QTimer *playTimer;
    int playIndex;
    int sampleIntervalMs;
    bool looping;

    QLineSeries *adcSeries;
    QChart *chart;
    QValueAxis *axisX;
    QValueAxis *axisY;

    QChartView *chartView;
    int lastIndex;

    // helpers
    void rebuildSamplesFromPoints(); // optional if you store raw points
};

class DrawChartView : public QChartView {

public:
    explicit DrawChartView(QChart *chart, UsrScopeWindow *owner);

protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;

private:
    UsrScopeWindow *owner;

};

#endif
