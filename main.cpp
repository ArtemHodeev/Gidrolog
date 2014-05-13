#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <databaseaccessor.h>
#include <dbcreator.h>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("DVO_RAN");
    QApplication::setApplicationName("Gydro");
//    QSettings *setting = new QSettings("DVO_RAN","Gydro");
//    setting->beginGroup("dbSettings");
//    setting->setValue("port",3306);
//    setting->endGroup();
//    DatabaseAccessor::dbHost = "localhost";
//    DatabaseAccessor::dbUser = "user";
//    DatabaseAccessor::dbPassword = "123";
//    DatabaseAccessor::dbPort = 3306;

    DBCreator db(DatabaseAccessor::getDb());
    db.checkSchema();

    MainWindow w;
    w.show();

    return a.exec();
}
