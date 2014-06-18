#ifndef EDITORNEW_H
#define EDITORNEW_H

#include <QMainWindow>
#include<tablemodel.h>
#include <QItemSelectionModel>
#include <itemmodel.h>
#include <itemtypemodel.h>
#include <watertypemodel.h>
#include <locationmodel.h>
#include <factormodel.h>
#include <checkboxdelegate.h>
#include <QPushButton>
#include <QTableView>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>

namespace Ui {
class EditorNew;
}

class EditorNew : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorNew(QWidget *parent = 0);
    ~EditorNew();

private:
    Ui::EditorNew *ui;

    QTabWidget *tabs;
    QPushButton *save_btn;
    QTableView *item_table;
    QTableView *item_type_table;
    QTableView *water_type_table;
    QTableView *location_table;
    QWidget *criteries;

    QLabel *analitic_type_label;
    QLabel *correl_label;
    QLabel *max_error_count_label;
    QLabel *count_of_lost_label;

    QComboBox *analitic_type_t;
    QLineEdit *correl_t;
    QLineEdit *max_error_t;
    QLineEdit *count_of_lost_t;

    TableModel *item_model;// модель данных таблицы
    TableModel *water_model;
    TableModel *location_model;
    TableModel *current_model;
    TableModel *item_type_model;
    FactorModel *factor_model;

    QItemSelectionModel *sel_model;// модель для выделения строк

    bool water_sign;
    bool location_sign;
    bool factor_sign;
    bool item_sign;
    bool item_type_sign;

    bool water_edited;
    bool location_edited;
    bool item_edited;
    bool factor_edited;  
    bool item_type_edited;

    void saveModel(TableModel* model);
    //void setUi(int index, TableModel *model);
    bool maybeSave();
    void save();
protected:
    void keyPressEvent(QKeyEvent *key_event);
    void closeEvent(QCloseEvent *event);
public slots:
    void tabBarClickedSlot(int index);
    void on_item_changed(const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void on_location_changed(const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void on_water_changed(const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void on_item_type_changed(const QModelIndex &topLeft, const QModelIndex &bottomRight);
signals:
    void exit_action();
private slots:
    void on_save_button_pressed();
    void on_comboBox_activated(const QString &arg1);
    void on_lineEdit_maxCorrel_textEdited(const QString &arg1);
    void on_lineEdit_maxErrorCount_textEdited(const QString &arg1);
    void on_lineEdit_countOfLost_textEdited(const QString &arg1);
};

#endif // EDITORNEW_H
