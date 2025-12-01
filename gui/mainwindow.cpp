#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QRegularExpression>
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(QSerialPort *serialPort, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serial(serialPort)
    , scopeWindow(nullptr)
{
    ui->setupUi(this);
    serial->setParent(this);
    
    // Connect signals
    connect(ui->startPWMBtn, &QPushButton::clicked, this, &MainWindow::onStartPWM);
    connect(ui->stopPWMBtn, &QPushButton::clicked, this, &MainWindow::onStopPWM);
    connect(ui->readADCBtn, &QPushButton::clicked, this, &MainWindow::onReadADC);
    connect(ui->startMonitorBtn, &QPushButton::clicked, this, &MainWindow::onStartMonitoring);
    connect(ui->stopMonitorBtn, &QPushButton::clicked, this, &MainWindow::onStopMonitoring);
    connect(ui->enablePIDBtn, &QPushButton::clicked, this, &MainWindow::onEnablePID);
    connect(ui->disablePIDBtn, &QPushButton::clicked, this, &MainWindow::onDisablePID);
    connect(ui->setFreqBtn, &QPushButton::clicked, this, &MainWindow::onSetFrequency);
    connect(ui->setDutyBtn, &QPushButton::clicked, this, &MainWindow::onSetDuty);
    connect(ui->setSetpointBtn, &QPushButton::clicked, this, &MainWindow::onSetSetpoint);
    connect(ui->showScopeBtn, &QPushButton::clicked, this, &MainWindow::onShowScope);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
}

MainWindow::~MainWindow()
{
    if (serial->isOpen()) serial->close();
    delete ui;
}

void MainWindow::onStartPWM()
{
    sendCommand("S 0\n");
}

void MainWindow::onStopPWM()
{
    sendCommand("T 0\n");
}

void MainWindow::onReadADC()
{
    sendCommand("R 0\n");
}

void MainWindow::onStartMonitoring()
{
    sendCommand("M 0\n");
}

void MainWindow::onStopMonitoring()
{
    sendCommand("N 0\n");
}

void MainWindow::onEnablePID()
{
    sendCommand("P 0\n");
}

void MainWindow::onDisablePID()
{
    sendCommand("Q 0\n");
}

void MainWindow::onSetFrequency()
{
    QString cmd = QString("F %1\n").arg(ui->freqSpin->value());
    sendCommand(cmd);
}

void MainWindow::onSetDuty()
{
    QString cmd = QString("D %1\n").arg(ui->dutySpin->value());
    sendCommand(cmd);
}

void MainWindow::onSetSetpoint()
{
    QString cmd = QString("V %1\n").arg(ui->setpointSpin->value());
    sendCommand(cmd);
}

void MainWindow::readSerialData()
{
    QByteArray data = serial->readAll();
    QString text = QString::fromUtf8(data);
    ui->terminalText->insertPlainText(text);
    ui->terminalText->ensureCursorVisible();
    
    QStringList lines = text.split('\n', Qt::SkipEmptyParts);

    for (const QString &line : lines) {
        QString trimmedLine = line.trimmed(); // removes spaces, tabs, \r
        if (!trimmedLine.isEmpty()) {        // skip truly blank lines
            parseAndPlotADC(trimmedLine);
        }
    }
}

void MainWindow::onShowScope()
{
    if (!scopeWindow) {
        scopeWindow = new ScopeWindow();
    }
    scopeWindow->show();
    scopeWindow->raise();
    scopeWindow->activateWindow();
}

void MainWindow::parseAndPlotADC(const QString &line)
{
    QString trimmed = line.trimmed();
    if (!trimmed.startsWith("ADC:")) return;
    
    // Require full format: "ADC: ### (#.##V)"
    QRegularExpression re("^ADC:\\s+(\\d+)\\s+\\(.*V\\)");
    QRegularExpressionMatch match = re.match(trimmed);
    if (match.hasMatch() && scopeWindow) {
        bool ok;
        int adcValue = match.captured(1).toInt(&ok);
        // Filter out spurious zeros and validate range
        if (ok && adcValue > 0 && adcValue <= 1023) {
            scopeWindow->addDataPoint(adcValue);
        }
    }
}

void MainWindow::sendCommand(const QString &cmd)
{
    if (serial->isOpen()) {
        serial->write(cmd.toUtf8());
        ui->terminalText->appendPlainText("> " + cmd.trimmed());
    }
}
