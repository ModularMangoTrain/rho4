#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include <QSerialPortInfo>
#include <QMessageBox>

ConnectionDialog::ConnectionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConnectionDialog)
    , serial(new QSerialPort(this))
{
    ui->setupUi(this);
    
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        ui->comPortCombo->addItem(info.portName());
    }
    
    ui->baudCombo->addItems({"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"});
    ui->baudCombo->setCurrentText("9600");
    
    ui->dataBitsCombo->addItems({"5", "6", "7", "8"});
    ui->dataBitsCombo->setCurrentText("8");
    
    ui->parityCombo->addItems({"None", "Even", "Odd", "Space", "Mark"});
    ui->parityCombo->setCurrentText("None");
    
    ui->stopBitsCombo->addItems({"1", "1.5", "2"});
    ui->stopBitsCombo->setCurrentText("1");
    
    connect(ui->connectBtn, &QPushButton::clicked, this, &ConnectionDialog::onConnectClicked);
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::onConnectClicked()
{
    serial->setPortName(ui->comPortCombo->currentText());
    serial->setBaudRate(ui->baudCombo->currentText().toInt());
    
    int dataBits = ui->dataBitsCombo->currentText().toInt();
    serial->setDataBits(static_cast<QSerialPort::DataBits>(dataBits));
    
    QString parity = ui->parityCombo->currentText();
    if (parity == "None") serial->setParity(QSerialPort::NoParity);
    else if (parity == "Even") serial->setParity(QSerialPort::EvenParity);
    else if (parity == "Odd") serial->setParity(QSerialPort::OddParity);
    else if (parity == "Space") serial->setParity(QSerialPort::SpaceParity);
    else if (parity == "Mark") serial->setParity(QSerialPort::MarkParity);
    
    QString stopBits = ui->stopBitsCombo->currentText();
    if (stopBits == "1") serial->setStopBits(QSerialPort::OneStop);
    else if (stopBits == "1.5") serial->setStopBits(QSerialPort::OneAndHalfStop);
    else if (stopBits == "2") serial->setStopBits(QSerialPort::TwoStop);
    
    if (serial->open(QIODevice::ReadWrite)) {
        accept();
    } else {
        QMessageBox::critical(this, "Error", "Failed to open port");
    }
}

QSerialPort* ConnectionDialog::getSerialPort()
{
    return serial;
}
