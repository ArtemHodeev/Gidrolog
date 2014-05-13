#include "connectdb.h"
#include "ui_connectdb.h"
#include <databaseaccessor.h>

ConnectDB::ConnectDB(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDB)
{
    ui->setupUi(this);

    setting = new QSettings();
    setting->beginGroup("dbSettings");
    ui->host_lineEdit->setText(setting->value("host").toString());
    ui->user_lineEdit->setText(setting->value("user").toString());
    ui->pass_lineEdit->setText(setting->value("password").toString());
    ui->port_lineEdit->setText(setting->value("port").toString());
    setting->endGroup();
}

ConnectDB::~ConnectDB()
{
    delete ui;
    delete setting;\
}

void ConnectDB::on_pushButton_pressed()
{
    QString host = (ui->host_lineEdit->text().compare("") != 0) ? ui->host_lineEdit->text() : "localhost";
    QString user = (ui->user_lineEdit->text().compare("") != 0) ? ui->user_lineEdit->text() : "user";
    QString pass = (ui->pass_lineEdit->text().compare("") != 0) ? ui->pass_lineEdit->text() : "123";
    int port = (ui->port_lineEdit->text().compare("") != 0) ? ui->port_lineEdit->text().toInt() : 3306;

    setting->beginGroup("dbSettings");
    setting->setValue("host", host);
    setting->setValue("user", user);
    setting->setValue("password", pass);
    setting->setValue("port", port);
    setting->endGroup();
    DatabaseAccessor::reconnect();

}
