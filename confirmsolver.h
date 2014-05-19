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
    QVector<unsigned int> getSelectedItems();

private slots:
    void on_pushButton_ok_pressed();

    void on_radioButton_selected_clicked();

    void on_radioButton_full_clicked();

    void on_listWidget_clicked(const QModelIndex &index);

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::ConfirmSolver *ui;
    int count;
    QVector<unsigned int> selected_items;
};

#endif // CONFIRMSOLVER_H
