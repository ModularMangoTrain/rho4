QT += core gui widgets serialport charts

TARGET = PWMController
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    connectiondialog.cpp \
    scopewindow.cpp \
    trajectorywindow.cpp

RESOURCES += \
    QDarkStyleSheet/qdarkstyle/dark/darkstyle.qrc

HEADERS += \
    mainwindow.h \
    connectiondialog.h \
    scopewindow.h \
    trajectorywindow.h

FORMS += \
    mainwindow.ui \
    connectiondialog.ui
