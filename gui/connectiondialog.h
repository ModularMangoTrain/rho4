#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class ConnectionDialog; }
QT_END_NAMESPACE

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    ConnectionDialog(QWidget *parent = nullptr);
    ~ConnectionDialog();
    QSerialPort* getSerialPort();

private slots:
    void onConnectClicked();

private:
    Ui::ConnectionDialog *ui;
    QSerialPort *serial;
};
#endif
