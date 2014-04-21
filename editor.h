#ifndef EDITOR_H
#define EDITOR_H

#include <QDialog>
#include<tablemodel.h>
#include <QItemSelectionModel>

namespace Ui {
class Editor;
}

class Editor : public QDialog
{
    Q_OBJECT
public:
    explicit Editor(QWidget *parent = 0);
    ~Editor();
protected:
    void keyPressEvent(QKeyEvent *key_event);
private:
    TableModel *model;
    QItemSelectionModel *sel_model;
    Ui::Editor *ui;
public slots:
    void on_listWidget_editorMenu_clicked();
    void on_saveButton_clicked();
signals:
    void save_clicked();
    void remove_clicked();
    void exit_action();
private slots:
    void on_pushButton_itemAdd_clicked();
    void on_pushButton_exit_clicked();
    void on_pushButton_itemRemove_clicked();
    void on_pushButton_exit_pressed();
};

#endif // EDITOR_H
