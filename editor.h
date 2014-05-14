#ifndef EDITOR_H
#define EDITOR_H

#include <QDialog>
#include<tablemodel.h>
#include <QItemSelectionModel>
#include <itemmodel.h>
#include <itemtypemodel.h>
#include <watertypemodel.h>
#include <locationmodel.h>
#include <factormodel.h>

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
    TableModel *item_model;// модель данных таблицы
    TableModel *water_model;
    TableModel *location_model;
    TableModel *current_model;
    TableModel *item_type_model;
    FactorModel *factor_model;

    QItemSelectionModel *sel_model;// модель для выделения строк
    Ui::Editor *ui;
    bool water_sign;
    bool location_sign;
    bool factor_sign;

    bool water_edited;
    bool location_edited;
    bool item_edited;
    bool factor_edited;
    bool item_sign;
    bool item_type_sign;
    bool item_type_edited;

    void saveModel(TableModel* model);
    void setUi(int index, TableModel *model);
    void closeEvent(QCloseEvent *event);
    bool maybeSave();
    void save();

public slots:
    void on_listWidget_editorMenu_clicked();
    void on_item_changed();
    void on_location_changed();
    void on_water_changed();
signals:
    void exit_action();
private slots:
    void on_pushButton_exit_pressed();
    void on_comboBox_activated(const QString &arg1);
    void on_lineEdit_maxCorrel_textEdited(const QString &arg1);
    void on_lineEdit_maxErrorCount_textEdited(const QString &arg1);
    void on_lineEdit_countOfLost_textEdited(const QString &arg1);
};

#endif // EDITOR_H
