#ifndef EDITOR_H
#define EDITOR_H

#include <QDialog>
#include<tablemodel.h>

namespace Ui {
class Editor;
}

class Editor : public QDialog
{
    Q_OBJECT
public:
    explicit Editor(QWidget *parent = 0);
    ~Editor();

private:
    TableModel *model;
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
};

#endif // EDITOR_H
