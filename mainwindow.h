#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <samplemodel.h>
#include <QItemSelectionModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void actionSave();
protected:
    void keyPressEvent(QKeyEvent *key_event);
private slots:
    void on_action_editorTool_triggered();
    void on_actionSave_triggered();
    void on_action_importExcel_triggered();

    void on_action_prepare_triggered();

    void on_action_3_triggered();

    void on_action_calcilate_triggered();

private:
    Ui::MainWindow *ui;
    SampleModel *model;// модель для проб
    QItemSelectionModel *sel_model; // модель для выделения (выделение строк)
};

#endif // MAINWINDOW_H
