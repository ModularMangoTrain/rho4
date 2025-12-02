#include "mainwindow.h"
#include "connectiondialog.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Load QDarkStyleSheet
    QFile styleFile(":/qdarkstyle/dark/darkstyle.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        a.setStyleSheet(styleFile.readAll());
    }
    
    ConnectionDialog connDialog;
    if (connDialog.exec() == QDialog::Accepted) {
        MainWindow w(connDialog.getSerialPort());
        w.show();
        return a.exec();
    }
    
    return 0;
}
