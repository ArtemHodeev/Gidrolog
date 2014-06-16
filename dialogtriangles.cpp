#include "dialogtriangles.h"
#include "ui_dialogtriangles.h"
#include <qapplication.h>
#include <QToolBar>
#include <QToolButton>
#include <baseflow.h>
#include <QMessageBox>

DialogTriangles::DialogTriangles(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DialogTriangles)
{
    ui->setupUi(this);
    plot = new Baseflow( this );
    plot->setTitle("Диаграммы смешения");
    plot->setMouseTracking(true);
    this->setMouseTracking(true);
    setCentralWidget( plot );

    QToolBar *toolBar = new QToolBar( this );

    QToolButton *btnExport = new QToolButton( toolBar );
    btnExport->setText( "Экспорт графика" );
    btnExport->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    connect( btnExport, SIGNAL( clicked() ), plot, SLOT( exportPlot() ) );

    QToolButton *btnShowAllItems = new QToolButton( toolBar );
    btnShowAllItems->setText( "Показать источники" );
    btnShowAllItems->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );

    QToolButton *btnDefault = new QToolButton( toolBar );
    btnDefault->setText( "Сбросить к исходному" );
    btnDefault->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    connect( btnDefault, SIGNAL( clicked() ), plot, SLOT( plotDefault() ) );

    QToolButton *btnMakeAnaliz = new QToolButton( toolBar );
    btnMakeAnaliz->setText( "Рассчитать" );
    btnMakeAnaliz->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    connect( btnMakeAnaliz, SIGNAL( clicked() ), plot, SLOT( makeAnaliz() ) );

    toolBar->addWidget( btnExport );
    toolBar->addWidget( btnShowAllItems );
    toolBar->addWidget( btnDefault );
    toolBar->addWidget( btnMakeAnaliz );
    addToolBar( toolBar );
}

DialogTriangles::~DialogTriangles()
{
    delete ui;
}
void DialogTriangles::mouseMoveEvent(QMouseEvent *event)
{
    double x = plot->invTransform(QwtPlot::xBottom, event->pos().x()) - 1.44676;
    double y = plot->invTransform(QwtPlot::yLeft, event->pos().y()) + 1.9954;
    statusBar()->showMessage("x= " + QString::number(x) + "; y = " + QString::number(y));
}
void DialogTriangles::closeEvent(QCloseEvent *event)
{
    if (askOnClose())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
bool DialogTriangles::askOnClose()
{
    QMessageBox msg;
    QPushButton  *yes = msg.addButton("Выйти",QMessageBox::AcceptRole);
    QPushButton  *no = msg.addButton("Отменить",QMessageBox::RejectRole);
    msg.setDefaultButton(yes);
    msg.setWindowTitle("Подтвердите выход");
    msg.setText("Вы действительно хотите покинуть окно построения диаграмм смешения?");
    msg.setIcon(QMessageBox::Question);
    return (msg.exec() == QMessageBox::AcceptRole) ? true : false;
}
