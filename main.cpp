#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <databaseaccessor.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseAccessor::dbName = "gydro";
    DatabaseAccessor::dbHost = "localhost";
    DatabaseAccessor::dbUser = "user";
    DatabaseAccessor::dbPassword = "123";
    DatabaseAccessor::dbPort = 5432;

	
    MainWindow w;
    w.show();

    return a.exec();
}
