#ifndef CONNECTDB_H
#define CONNECTDB_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class ConnectDB;
}

class ConnectDB : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDB(QWidget *parent = 0);
    ~ConnectDB();

private slots:
    void on_pushButton_pressed();

private:
    Ui::ConnectDB *ui;
    QSettings *setting;

};

#endif // CONNECTDB_H
