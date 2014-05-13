#include "connectdb.h"
#include "ui_connectdb.h"

ConnectDB::ConnectDB(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDB)
{
    ui->setupUi(this);
}

ConnectDB::~ConnectDB()
{
    delete ui;
}
