QT       += core gui
QT += axcontainer
QT += widgets printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

#设置字符
QMAKE_CXXFLAGS += -execution-charset:utf-8
QMAKE_CXXFLAGS += -source-charset:utf-8
QMAKE_CXXFLAGS_WARN_ON += -wd4819

#包含子模块
include (./HikSdk/HikSdk.pri)   #海康SDK

SOURCES += \
    excelmanger.cpp \
    exceloperator.cpp \
    main.cpp \
    mainwindow.cpp \
    myglwidget.cpp \
    qcustomplot.cpp

HEADERS += \
    excelmanger.h \
    exceloperator.h \
    mainwindow.h \
    myglwidget.h \
    qcustomplot.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

