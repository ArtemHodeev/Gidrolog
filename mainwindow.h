#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void on_action_editorTool_triggered();
    void on_actionSave_triggered();
signals:
    void actionSave();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
