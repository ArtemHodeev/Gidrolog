#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <databaseaccessor.h>
#include <dbcreator.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    DatabaseAccessor::dbName = "gydro";
    DatabaseAccessor::dbHost = "localhost";
    DatabaseAccessor::dbUser = "user";
    DatabaseAccessor::dbPassword = "123";
//    DatabaseAccessor::dbPort = 5432;
    DatabaseAccessor::dbPort = 3306;

//    DatabaseAccessor db;
//    db.checkSchema();
    DBCreator db(DatabaseAccessor::getDb());
    db.checkSchema();
//    enum table{item, item_type, water_type,location,sample_set,sample,item_sample,factor};
//    if (QString("item").compare(table) == 0)
//    qDebug()<<table(QString("item"));
    MainWindow w;
    w.show();

    return a.exec();
}
