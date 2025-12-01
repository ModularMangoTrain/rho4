#include "mainwindow.h"
#include "connectiondialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    ConnectionDialog connDialog;
    if (connDialog.exec() == QDialog::Accepted) {
        MainWindow w(connDialog.getSerialPort());
        w.show();
        return a.exec();
    }
    
    return 0;
}
