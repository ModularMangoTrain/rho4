/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *vboxLayout;
    QGroupBox *pwmGroup;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpinBox *freqSpin;
    QPushButton *setFreqBtn;
    QLabel *label1;
    QSpinBox *dutySpin;
    QPushButton *setDutyBtn;
    QPushButton *stopPWMBtn;
    QPushButton *startPWMBtn;
    QLabel *label_3;
    QGroupBox *adcGroup;
    QHBoxLayout *hboxLayout;
    QPushButton *readADCBtn;
    QPushButton *startMonitorBtn;
    QPushButton *stopMonitorBtn;
    QGroupBox *pidGroup;
    QGridLayout *gridLayout1;
    QLabel *label2;
    QSpinBox *setpointSpin;
    QPushButton *setSetpointBtn;
    QPushButton *disablePIDBtn;
    QPushButton *enablePIDBtn;
    QLabel *label_2;
    QPushButton *showScopeBtn;
    QGroupBox *terminalGroup;
    QVBoxLayout *vboxLayout1;
    QPlainTextEdit *terminalText;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        vboxLayout = new QVBoxLayout(centralwidget);
        vboxLayout->setObjectName("vboxLayout");
        pwmGroup = new QGroupBox(centralwidget);
        pwmGroup->setObjectName("pwmGroup");
        gridLayout = new QGridLayout(pwmGroup);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(pwmGroup);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        freqSpin = new QSpinBox(pwmGroup);
        freqSpin->setObjectName("freqSpin");
        freqSpin->setMaximum(5000);
        freqSpin->setValue(1000);

        gridLayout->addWidget(freqSpin, 0, 1, 1, 1);

        setFreqBtn = new QPushButton(pwmGroup);
        setFreqBtn->setObjectName("setFreqBtn");

        gridLayout->addWidget(setFreqBtn, 0, 2, 1, 1);

        label1 = new QLabel(pwmGroup);
        label1->setObjectName("label1");

        gridLayout->addWidget(label1, 1, 0, 1, 1);

        dutySpin = new QSpinBox(pwmGroup);
        dutySpin->setObjectName("dutySpin");
        dutySpin->setMaximum(100);
        dutySpin->setValue(50);

        gridLayout->addWidget(dutySpin, 1, 1, 1, 1);

        setDutyBtn = new QPushButton(pwmGroup);
        setDutyBtn->setObjectName("setDutyBtn");

        gridLayout->addWidget(setDutyBtn, 1, 2, 1, 1);

        stopPWMBtn = new QPushButton(pwmGroup);
        stopPWMBtn->setObjectName("stopPWMBtn");

        gridLayout->addWidget(stopPWMBtn, 2, 2, 1, 1);

        startPWMBtn = new QPushButton(pwmGroup);
        startPWMBtn->setObjectName("startPWMBtn");

        gridLayout->addWidget(startPWMBtn, 2, 1, 1, 1);

        label_3 = new QLabel(pwmGroup);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 2, 0, 1, 1);


        vboxLayout->addWidget(pwmGroup);

        adcGroup = new QGroupBox(centralwidget);
        adcGroup->setObjectName("adcGroup");
        hboxLayout = new QHBoxLayout(adcGroup);
        hboxLayout->setObjectName("hboxLayout");
        readADCBtn = new QPushButton(adcGroup);
        readADCBtn->setObjectName("readADCBtn");

        hboxLayout->addWidget(readADCBtn);

        startMonitorBtn = new QPushButton(adcGroup);
        startMonitorBtn->setObjectName("startMonitorBtn");

        hboxLayout->addWidget(startMonitorBtn);

        stopMonitorBtn = new QPushButton(adcGroup);
        stopMonitorBtn->setObjectName("stopMonitorBtn");

        hboxLayout->addWidget(stopMonitorBtn);


        vboxLayout->addWidget(adcGroup);

        pidGroup = new QGroupBox(centralwidget);
        pidGroup->setObjectName("pidGroup");
        gridLayout1 = new QGridLayout(pidGroup);
        gridLayout1->setObjectName("gridLayout1");
        label2 = new QLabel(pidGroup);
        label2->setObjectName("label2");

        gridLayout1->addWidget(label2, 0, 0, 1, 1);

        setpointSpin = new QSpinBox(pidGroup);
        setpointSpin->setObjectName("setpointSpin");
        setpointSpin->setMaximum(1023);
        setpointSpin->setValue(100);

        gridLayout1->addWidget(setpointSpin, 0, 1, 1, 1);

        setSetpointBtn = new QPushButton(pidGroup);
        setSetpointBtn->setObjectName("setSetpointBtn");

        gridLayout1->addWidget(setSetpointBtn, 0, 2, 1, 1);

        disablePIDBtn = new QPushButton(pidGroup);
        disablePIDBtn->setObjectName("disablePIDBtn");

        gridLayout1->addWidget(disablePIDBtn, 1, 2, 1, 1);

        enablePIDBtn = new QPushButton(pidGroup);
        enablePIDBtn->setObjectName("enablePIDBtn");

        gridLayout1->addWidget(enablePIDBtn, 1, 1, 1, 1);

        label_2 = new QLabel(pidGroup);
        label_2->setObjectName("label_2");

        gridLayout1->addWidget(label_2, 1, 0, 1, 1);


        vboxLayout->addWidget(pidGroup);

        showScopeBtn = new QPushButton(centralwidget);
        showScopeBtn->setObjectName("showScopeBtn");

        vboxLayout->addWidget(showScopeBtn);

        terminalGroup = new QGroupBox(centralwidget);
        terminalGroup->setObjectName("terminalGroup");
        vboxLayout1 = new QVBoxLayout(terminalGroup);
        vboxLayout1->setObjectName("vboxLayout1");
        terminalText = new QPlainTextEdit(terminalGroup);
        terminalText->setObjectName("terminalText");
        terminalText->setReadOnly(true);

        vboxLayout1->addWidget(terminalText);


        vboxLayout->addWidget(terminalGroup);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "PWM PID Controller", nullptr));
        pwmGroup->setTitle(QCoreApplication::translate("MainWindow", "PWM Control", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Frequency (Hz):", nullptr));
        setFreqBtn->setText(QCoreApplication::translate("MainWindow", "Set", nullptr));
        label1->setText(QCoreApplication::translate("MainWindow", "Duty Cycle (%):", nullptr));
        setDutyBtn->setText(QCoreApplication::translate("MainWindow", "Set", nullptr));
        stopPWMBtn->setText(QCoreApplication::translate("MainWindow", "Stop PWM", nullptr));
        startPWMBtn->setText(QCoreApplication::translate("MainWindow", "Start PWM", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Enable/Disable PWM", nullptr));
        adcGroup->setTitle(QCoreApplication::translate("MainWindow", "ADC Monitoring", nullptr));
        readADCBtn->setText(QCoreApplication::translate("MainWindow", "Read ADC", nullptr));
        startMonitorBtn->setText(QCoreApplication::translate("MainWindow", "Start Monitor", nullptr));
        stopMonitorBtn->setText(QCoreApplication::translate("MainWindow", "Stop Monitor", nullptr));
        pidGroup->setTitle(QCoreApplication::translate("MainWindow", "PID Control", nullptr));
        label2->setText(QCoreApplication::translate("MainWindow", "Setpoint (ADC):", nullptr));
        setSetpointBtn->setText(QCoreApplication::translate("MainWindow", "Set", nullptr));
        disablePIDBtn->setText(QCoreApplication::translate("MainWindow", "Disable PID", nullptr));
        enablePIDBtn->setText(QCoreApplication::translate("MainWindow", "Enable PID", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Enable/Disable PID", nullptr));
        showScopeBtn->setText(QCoreApplication::translate("MainWindow", "Show Oscilloscope", nullptr));
        terminalGroup->setTitle(QCoreApplication::translate("MainWindow", "Terminal Output", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
