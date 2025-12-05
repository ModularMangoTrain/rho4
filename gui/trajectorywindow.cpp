#include "trajectorywindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSerialPort>
#include <QMessageBox>
#include <QDebug>
#include <QThread>

TrajectoryWindow::TrajectoryWindow(QSerialPort *serialPort, QWidget *parent)
    : QWidget(parent)
    , serial(serialPort)
    , isDrawing(false)
    , lastDrawIndex(-1)
    , cursorInDrawArea(false)
{
    setWindowTitle("Draw Trajectory");
    setMinimumSize(800, 600);
    resize(800, 600);
    setMouseTracking(true);
    
    // Create buttons
    clearBtn = new QPushButton("Clear");
    sendBtn = new QPushButton("Send to Device");
    startBtn = new QPushButton("Start Tracking");
    stopBtn = new QPushButton("Stop Tracking");
    
    clearBtn->setMinimumHeight(40);
    sendBtn->setMinimumHeight(40);
    startBtn->setMinimumHeight(40);
    stopBtn->setMinimumHeight(40);
    
    // Button layout
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(clearBtn);
    btnLayout->addWidget(sendBtn);
    btnLayout->addWidget(startBtn);
    btnLayout->addWidget(stopBtn);
    
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addLayout(btnLayout);
    
    // Connect signals
    connect(clearBtn, &QPushButton::clicked, this, &TrajectoryWindow::onClear);
    connect(sendBtn, &QPushButton::clicked, this, &TrajectoryWindow::onSendTrajectory);
    connect(startBtn, &QPushButton::clicked, this, &TrajectoryWindow::onStartTracking);
    connect(stopBtn, &QPushButton::clicked, this, &TrajectoryWindow::onStopTracking);
    
    setStyleSheet("background-color: #192330;");
}

TrajectoryWindow::~TrajectoryWindow()
{
}

void TrajectoryWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    int drawHeight = height() - 80;  // Leave more space for buttons
    
    // Draw background
    painter.fillRect(0, 0, width(), drawHeight, QColor(35, 45, 55));
    painter.fillRect(0, drawHeight, width(), 80, QColor(25, 35, 45));
    
    // Draw grid
    painter.setPen(QPen(QColor(60, 70, 80), 1));
    int gridSpacing = 50;
    for (int x = 0; x < width(); x += gridSpacing) {
        painter.drawLine(x, 0, x, drawHeight);
    }
    for (int y = 0; y < drawHeight; y += gridSpacing) {
        painter.drawLine(0, y, width(), y);
    }
    
    // Draw reference lines and labels
    painter.setPen(QPen(QColor(100, 110, 120), 2, Qt::DashLine));
    int midY = drawHeight / 2;
    painter.drawLine(0, midY, width(), midY);
    
    // Draw ADC value labels
    painter.setPen(QColor(200, 210, 220));
    QFont labelFont = painter.font();
    labelFont.setPointSize(12);
    labelFont.setBold(true);
    painter.setFont(labelFont);
    painter.drawText(10, 25, "1023 (3.3V) ← TOP");
    painter.drawText(10, midY + 5, "512 (1.65V) ← MIDDLE");
    painter.drawText(10, drawHeight - 10, "0 (0V) ← BOTTOM");
    
    // Draw instructions if no points
    if (samples.isEmpty()) {
        painter.setPen(QColor(150, 160, 170));
        QFont font = painter.font();
        font.setPointSize(16);
        painter.setFont(font);
        QRect textRect(0, 0, width(), drawHeight);
        painter.drawText(textRect, Qt::AlignCenter, 
                        "Click and drag to draw your waveform\n\nTop = 1023 (3.3V)\nMiddle = 512 (1.65V)\nBottom = 0 (0V)");
    }
    
    // Draw the trajectory
    if (!samples.isEmpty()) {
        painter.setPen(QPen(QColor(0, 188, 212), 2));
        for (int x = 1; x < samples.size() && x < width(); ++x) {
            int y1 = drawHeight - 1 - ((samples[x-1] * (drawHeight - 1)) / 1023);
            int y2 = drawHeight - 1 - ((samples[x] * (drawHeight - 1)) / 1023);
            painter.drawLine(x-1, y1, x, y2);
        }
    }
    
    // Draw cursor ADC value
    if (cursorInDrawArea) {
        int adcValue = yToADC(cursorPos.y());
        painter.setPen(QColor(255, 200, 0));
        QFont cursorFont = painter.font();
        cursorFont.setPointSize(14);
        cursorFont.setBold(true);
        painter.setFont(cursorFont);
        QString adcText = QString("ADC: %1").arg(adcValue);
        painter.drawText(cursorPos.x() + 15, cursorPos.y() - 10, adcText);
        
        // Draw crosshair
        painter.setPen(QPen(QColor(255, 200, 0), 1, Qt::DashLine));
        painter.drawLine(0, cursorPos.y(), width(), cursorPos.y());
        painter.drawLine(cursorPos.x(), 0, cursorPos.x(), drawHeight);
    }
}

void TrajectoryWindow::mousePressEvent(QMouseEvent *event)
{
    int drawHeight = height() - 80;
    if (event->button() == Qt::LeftButton && event->pos().y() >= 0 && event->pos().y() < drawHeight) {
        isDrawing = true;
        if (samples.isEmpty())
            samples.fill(512, width());
        lastDrawIndex = -1;
        mouseMoveEvent(event);
    }
}

void TrajectoryWindow::mouseMoveEvent(QMouseEvent *event)
{
    int drawHeight = height() - 80;
    cursorPos = event->pos();
    cursorInDrawArea = (cursorPos.y() >= 0 && cursorPos.y() < drawHeight);
    
    if (isDrawing) {
        if (samples.isEmpty())
            samples.fill(512, width());
        
        int idx = xToIndex(event->pos().x());
        int adcVal = yToADC(event->pos().y());
        
        if (lastDrawIndex < 0) lastDrawIndex = idx;
        
        int a = qMin(lastDrawIndex, idx);
        int b = qMax(lastDrawIndex, idx);
        
        for (int i = a; i <= b; i++) {
            float t = (b == a ? 1.0f : float(i - a) / float(b - a));
            samples[i] = samples[lastDrawIndex] + t * (adcVal - samples[lastDrawIndex]);
        }
        
        lastDrawIndex = idx;
    }
    update();
}

void TrajectoryWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDrawing = false;
        lastDrawIndex = -1;
    }
}

void TrajectoryWindow::onClear()
{
    samples.clear();
    sendCommand("C 0\n");
    update();
}

void TrajectoryWindow::onSendTrajectory()
{
    if (samples.isEmpty()) {
        QMessageBox::warning(this, "No Trajectory", "Please draw a trajectory first!");
        return;
    }
    
    // Resample to 200 points
    QVector<int> toSend;
    int numSamples = qMin(200, samples.size());
    for (int i = 0; i < numSamples; ++i) {
        int idx = (i * (samples.size() - 1)) / (numSamples - 1);
        toSend.append(samples[idx]);
    }
    
    sendCommand("C 0\n");
    QThread::msleep(100);
    
    for (int adcValue : toSend) {
        QString cmd = QString("A %1\n").arg(adcValue);
        sendCommand(cmd);
        QThread::msleep(20);
    }
    
    QMessageBox::information(this, "Success", QString("Sent %1 points").arg(toSend.size()));
}

void TrajectoryWindow::onStartTracking()
{
    if (samples.isEmpty()) {
        QMessageBox::warning(this, "No Trajectory", "Please draw and send a trajectory first!");
        return;
    }
    
    sendCommand("P 0\n");
    sendCommand("W 0\n");
    QMessageBox::information(this, "Tracking Started", "PID is now tracking your drawn trajectory!");
}

void TrajectoryWindow::onStopTracking()
{
    sendCommand("E 0\n");  // End trajectory mode
    sendCommand("Q 0\n");  // Disable PID
}

void TrajectoryWindow::sendCommand(const QString &cmd)
{
    if (serial && serial->isOpen()) {
        QByteArray data = cmd.toUtf8();
        qDebug() << "Sending:" << data.toHex(' ') << "ASCII:" << cmd.trimmed();
        serial->write(data);
        serial->flush();
    }
}

int TrajectoryWindow::xToIndex(int x) const
{
    if (samples.isEmpty()) return 0;
    return qBound(0, x, samples.size() - 1);
}

int TrajectoryWindow::yToADC(int y)
{
    int drawHeight = height() - 80;
    if (drawHeight <= 0) drawHeight = 1;
    int clampedY = qBound(0, y, drawHeight - 1);
    int adcValue = 1023 - ((clampedY * 1023) / (drawHeight - 1));
    return qBound(0, adcValue, 1023);
}
