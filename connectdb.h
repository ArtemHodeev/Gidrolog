#ifndef CONNECTDB_H
#define CONNECTDB_H

#include <QDialog>

namespace Ui {
class ConnectDB;
}

class ConnectDB : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDB(QWidget *parent = 0);
    ~ConnectDB();

private:
    Ui::ConnectDB *ui;
};

#endif // CONNECTDB_H
