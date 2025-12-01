QT += core gui widgets serialport charts

TARGET = PWMController
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    connectiondialog.cpp \
    scopewindow.cpp

HEADERS += \
    mainwindow.h \
    connectiondialog.h \
    scopewindow.h

FORMS += \
    mainwindow.ui \
    connectiondialog.ui
