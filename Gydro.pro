#-------------------------------------------------
#
# Project created by QtCreator 2014-02-21T16:38:06
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(xlsx\qtxlsx.pri)

TARGET = Gydro
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    editor.cpp \
    connector.cpp \
    pgconnector.cpp \
    tablemodel.cpp \
    databaseaccessor.cpp \
    item.cpp \
    itemtypemodel.cpp \
    itemtype.cpp \
    sample.cpp \
    samplemodel.cpp \
    iteminsample.cpp \
    itemmodel.cpp \
    importer.cpp

HEADERS  += mainwindow.h \
    editor.h \
    connector.h \
    pgconnector.h \
    tablemodel.h \
    databaseaccessor.h \
    item.h \
    itemtypemodel.h \
    itemtype.h \
    sample.h \
    samplemodel.h \
    iteminsample.h \
    itemmodel.h \
    importer.h

FORMS    += mainwindow.ui \
    editor.ui
