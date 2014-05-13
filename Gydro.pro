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
    importer.cpp \
    dbcreator.cpp \
    watertype.cpp \
    watertypemodel.cpp \
    confirmimport.cpp \
    comboboxdelegate.cpp \
    names.cpp \
    watertypecombobox.cpp \
    locationcombobox.cpp \
    confirmitemmodel.cpp \
    confirmwatertypemodel.cpp \
    location.cpp \
    locationmodel.cpp \
    confirmlocationmodel.cpp \
    factormodel.cpp \
    calculator.cpp \
    iteminfo.cpp \
    confirmcalculator.cpp \
    confirmcalculatormodel.cpp \
    itemcorellation.cpp \
    connectdb.cpp

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
    importer.h \
    dbcreator.h \
    watertype.h \
    watertypemodel.h \
    confirmimport.h \
    comboboxdelegate.h \
    names.h \
    watertypecombobox.h \
    locationcombobox.h \
    confirmitemmodel.h \
    confirmwatertypemodel.h \
    location.h \
    locationmodel.h \
    confirmlocationmodel.h \
    factormodel.h \
    calculator.h \
    iteminfo.h \
    confirmcalculator.h \
    confirmcalculatormodel.h \
    itemcorellation.h \
    connectdb.h

FORMS    += mainwindow.ui \
    editor.ui \
    confirmimport.ui \
    confirmcalculator.ui \
    connectdb.ui
