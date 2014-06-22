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
    connectdb.cpp \
    itemtypecombobox.cpp \
    spinboxdelegate.cpp \
    checkboxdelegate.cpp \
    solverpca.cpp \
    confirmsolver.cpp \
    alglib/alglibinternal.cpp \
    alglib/alglibmisc.cpp \
    alglib/ap.cpp \
    alglib/dataanalysis.cpp \
    alglib/diffequations.cpp \
    alglib/fasttransforms.cpp \
    alglib/integration.cpp \
    alglib/interpolation.cpp \
    alglib/linalg.cpp \
    alglib/optimization.cpp \
    alglib/solvers.cpp \
    alglib/specialfunctions.cpp \
    alglib/statistics.cpp \
    standartization.cpp \
    alltriangles.cpp \
    gentriangles.cpp \
    doubleboxdelegate.cpp \
    dialogtriangles.cpp \
    baseflow.cpp \
    sampleinfo.cpp \
    analiticpoints.cpp \
    springpoints.cpp \
    testsolve.cpp \
    editornew.cpp \
    sampleinfoparser.cpp \
    mypicker.cpp \
    starter.cpp

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
    connectdb.h \
    itemtypecombobox.h \
    spinboxdelegate.h \
    checkboxdelegate.h \
    solverpca.h \
    confirmsolver.h \
    alglib/alglibinternal.h \
    alglib/alglibmisc.h \
    alglib/ap.h \
    alglib/dataanalysis.h \
    alglib/diffequations.h \
    alglib/fasttransforms.h \
    alglib/integration.h \
    alglib/interpolation.h \
    alglib/linalg.h \
    alglib/optimization.h \
    alglib/solvers.h \
    alglib/specialfunctions.h \
    alglib/statistics.h \
    standartization.h \
    alltriangles.h \
    gentriangles.h \
    doubleboxdelegate.h \
    dialogtriangles.h \
    baseflow.h \
    sampleinfo.h \
    analiticpoints.h \
    springpoints.h \
    testsolve.h \
    editornew.h \
    sampleinfoparser.h \
    mypicker.h \
    starter.h

FORMS    += mainwindow.ui \
    editor.ui \
    confirmimport.ui \
    confirmcalculator.ui \
    connectdb.ui \
    confirmsolver.ui \
    dialogtriangles.ui \
    testsolve.ui \
    editornew.ui
CONFIG += qwt

QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp

RESOURCES += \
    resources.qrc
