#ifndef CONFIRMIMPORT_H
#define CONFIRMIMPORT_H

#include <QDialog>

namespace Ui {
class ConfirmImport;
}

class ConfirmImport : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmImport(QWidget *parent = 0);
    ~ConfirmImport();

private:
    Ui::ConfirmImport *ui;
};

#endif // CONFIRMIMPORT_H
