#ifndef TRAJECTORYWINDOW_H
#define TRAJECTORYWINDOW_H

#include <QWidget>
#include <QVector>
#include <QPoint>

class QSerialPort;
class QPushButton;

class TrajectoryWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TrajectoryWindow(QSerialPort *serialPort, QWidget *parent = nullptr);
    ~TrajectoryWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void onClear();
    void onSendTrajectory();
    void onStartTracking();
    void onStopTracking();

private:
    QSerialPort *serial;
    QVector<int> samples;  // ADC values 0-1023, one per X pixel
    bool isDrawing;
    int lastDrawIndex;
    QPushButton *clearBtn;
    QPushButton *sendBtn;
    QPushButton *startBtn;
    QPushButton *stopBtn;
    QPoint cursorPos;
    bool cursorInDrawArea;
    
    void sendCommand(const QString &cmd);
    int yToADC(int y);
    int xToIndex(int x) const;
    void updateDrawing();
};

#endif
