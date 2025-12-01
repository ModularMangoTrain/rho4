/********************************************************************************
** Form generated from reading UI file 'connectiondialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTIONDIALOG_H
#define UI_CONNECTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConnectionDialog
{
public:
    QVBoxLayout *vboxLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QComboBox *comPortCombo;
    QLabel *label1;
    QComboBox *baudCombo;
    QLabel *label2;
    QComboBox *dataBitsCombo;
    QLabel *label3;
    QComboBox *parityCombo;
    QLabel *label4;
    QComboBox *stopBitsCombo;
    QPushButton *connectBtn;

    void setupUi(QDialog *ConnectionDialog)
    {
        if (ConnectionDialog->objectName().isEmpty())
            ConnectionDialog->setObjectName("ConnectionDialog");
        ConnectionDialog->resize(400, 250);
        vboxLayout = new QVBoxLayout(ConnectionDialog);
        vboxLayout->setObjectName("vboxLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label = new QLabel(ConnectionDialog);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label);

        comPortCombo = new QComboBox(ConnectionDialog);
        comPortCombo->setObjectName("comPortCombo");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, comPortCombo);

        label1 = new QLabel(ConnectionDialog);
        label1->setObjectName("label1");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label1);

        baudCombo = new QComboBox(ConnectionDialog);
        baudCombo->setObjectName("baudCombo");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, baudCombo);

        label2 = new QLabel(ConnectionDialog);
        label2->setObjectName("label2");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label2);

        dataBitsCombo = new QComboBox(ConnectionDialog);
        dataBitsCombo->setObjectName("dataBitsCombo");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, dataBitsCombo);

        label3 = new QLabel(ConnectionDialog);
        label3->setObjectName("label3");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label3);

        parityCombo = new QComboBox(ConnectionDialog);
        parityCombo->setObjectName("parityCombo");

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, parityCombo);

        label4 = new QLabel(ConnectionDialog);
        label4->setObjectName("label4");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, label4);

        stopBitsCombo = new QComboBox(ConnectionDialog);
        stopBitsCombo->setObjectName("stopBitsCombo");

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, stopBitsCombo);


        vboxLayout->addLayout(formLayout);

        connectBtn = new QPushButton(ConnectionDialog);
        connectBtn->setObjectName("connectBtn");

        vboxLayout->addWidget(connectBtn);


        retranslateUi(ConnectionDialog);

        QMetaObject::connectSlotsByName(ConnectionDialog);
    } // setupUi

    void retranslateUi(QDialog *ConnectionDialog)
    {
        ConnectionDialog->setWindowTitle(QCoreApplication::translate("ConnectionDialog", "Connect to Device", nullptr));
        label->setText(QCoreApplication::translate("ConnectionDialog", "COM Port:", nullptr));
        label1->setText(QCoreApplication::translate("ConnectionDialog", "Baud Rate:", nullptr));
        label2->setText(QCoreApplication::translate("ConnectionDialog", "Data Bits:", nullptr));
        label3->setText(QCoreApplication::translate("ConnectionDialog", "Parity:", nullptr));
        label4->setText(QCoreApplication::translate("ConnectionDialog", "Stop Bits:", nullptr));
        connectBtn->setText(QCoreApplication::translate("ConnectionDialog", "Connect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConnectionDialog: public Ui_ConnectionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTIONDIALOG_H
