#ifndef CONFIRMSOLVER_H
#define CONFIRMSOLVER_H

#include <QDialog>

namespace Ui {
class ConfirmSolver;
}

class ConfirmSolver : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmSolver(QWidget *parent = 0);
    ~ConfirmSolver();
    int getCount() const;

private slots:
    void on_pushButton_ok_pressed();

private:
    Ui::ConfirmSolver *ui;
    int count;

};

#endif // CONFIRMSOLVER_H
