#include "dialogtriangles.h"
#include "ui_dialogtriangles.h"
#include <qapplication.h>
#include <QToolBar>
#include <QToolButton>
#include <baseflow.h>
#include <QMessageBox>
#include <QListWidget>
#include <QGridLayout>

DialogTriangles::DialogTriangles(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DialogTriangles)
{
    ui->setupUi(this);

    layout_triangles = new QBoxLayout(QBoxLayout::LeftToRight);
    itemslist = new QListWidget();
    itemslist->setMaximumWidth(170);
    connect( itemslist, SIGNAL(clicked(QModelIndex)), this, SLOT( on_listWidget_clicked(const QModelIndex &) ) );

    layout_triangles->addWidget(itemslist, 1);

    plot = new Baseflow( this );
    plot->setTitle("Диаграммы смешения");
    //setCentralWidget( plot );

    layout_triangles->addWidget(plot, 2);
    centralWidget()->setLayout(layout_triangles);
    centralWidget()->setMouseTracking(true);
    plot->setMouseTracking(true);
    this->setMouseTracking(true);

    QToolBar *toolBar = new QToolBar( this );

    QToolButton *btnExport = new QToolButton( toolBar );
    btnExport->setText( "Экспорт графика" );
    btnExport->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    connect( btnExport, SIGNAL( clicked() ), plot, SLOT( exportPlot() ) );

    QToolButton *btnDefault = new QToolButton( toolBar );
    btnDefault->setText( "Сбросить к исходному" );
    btnDefault->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    connect( btnDefault, SIGNAL( clicked() ), plot, SLOT( plotDefault() ) );

    QToolButton *btnMakeAnaliz = new QToolButton( toolBar );
    btnMakeAnaliz->setText( "Рассчитать" );
    btnMakeAnaliz->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    connect( btnMakeAnaliz, SIGNAL( clicked() ), plot, SLOT( makeAnaliz() ) );

    toolBar->addWidget( btnExport );
    toolBar->addWidget( btnDefault );
    toolBar->addWidget( btnMakeAnaliz );
    addToolBar( toolBar );
}

DialogTriangles::~DialogTriangles()
{
    delete ui;
}
//void DialogTriangles::mouseMoveEvent(QMouseEvent *event)
//{
//    double x = plot->invTransform(QwtPlot::xBottom, event->pos().x());
//    double y = plot->invTransform(QwtPlot::yLeft, event->pos().y());
//    statusBar()->showMessage("x= " + QString::number(x) + "; y = " + QString::number(y));
//}
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
void DialogTriangles::setSamples(QVector<Sample *> s)
{
    solve.setItems(s);
    solve.standart();
}
void DialogTriangles::setParam(int count, QVector<unsigned int> cust_items)
{
    QHash<QString, unsigned int>::iterator iter;
    QVector<unsigned int> mass;
    QHash<unsigned int*, double**>::iterator plur_iter;
    for (iter = Names::params->begin(); iter != Names::params->end(); iter ++)
    {
        mass.append(iter.value());
    }
    this->count = count;
    solve.makePlurals(count,cust_items, mass);
    for (plur_iter = solve.complete_plurals.begin(); plur_iter != solve.complete_plurals.end(); plur_iter ++)
    {
        plurals.append(plur_iter.key());
    }
}
void DialogTriangles::setPlurList()
{
    QString str = "";
    QStringList list;

    for (int i = 0; i < plurals.size(); i ++)
    {
        str = "< ";
        for (int j = 0; j < count; j ++)
        {
            str += QString("%1 ").arg(Names::params->key(plurals[i][j]));
        }
        str += ">";
        list.append(str);

    }
    itemslist->addItems(list);
}
void DialogTriangles::on_listWidget_clicked(const QModelIndex &index)
{
    plot->detachItems(QwtPlotItem::Rtti_PlotItem, true);  // Очистка рабочей области
    QHash<QString, unsigned int>::iterator iter;
    QVector<SampleInfo> si;             //Проба воды
    QVector<SampleInfo> bi;             //Источники питания (точки треуголтника)
    QVector<SampleInfo> all_bi;
    AllTriangles one;
    qDebug()<<"index.row(): "<<index.row();
    for (iter = Names::water_types->begin(); iter != Names::water_types->end(); iter++)
    {
        double ave_u1 = 0;
        double ave_u2 = 0;
        qDebug()<<"plurals[index.row]: "<< plurals[index.row()];
        si = solve.getSamplesInfo(plurals[index.row()],iter.value());
        for (int i = 0; i < si.size(); i ++)
        {
            qDebug() << "Тип водной массы: " << Names::water_types->key(si[i].getWaterId()) << "U1"<< si[i].getU1() << "U2" << si[i].getU2();
            ave_u1 += si[i].getU1();    //высчитали среднее для U1
            ave_u2 += si[i].getU2();    //высчитали среднее для U2
         }
        if (iter.value() == Names::analitic_id)
        {
            one.setSamplesX(si);
            one.setSamplesY(si);
        }
        else
        {
            all_bi += si;
            SampleInfo temp_info;
            if (ave_u1 != 0 || ave_u2 != 0)
            {
                temp_info.setU1(ave_u1/si.size());
                temp_info.setU2(ave_u2/si.size());
                qDebug() << "Среднее для U1: " << ave_u1 << "для " << iter.key();
                qDebug() << "Среднее для U2: " << ave_u2;
                temp_info.setWaterId(iter.value());
                bi.append(temp_info);
            }
        }
    }
//    SampleInfo new_all_baseflow;
//    for (int i=0;i<100;i++)
//    {
//        new_all_baseflow.setU1(rand()%10);
//        new_all_baseflow.setU2(rand()%10);
//        new_all_baseflow.setLocationId(i%3 + 1);
//        new_all_baseflow.setWaterId(i%6 + 1);
//        new_all_baseflow.setDate(QDateTime::fromString("2012-12-12 12:12", "yyyy-MM-dd hh:mm"));
//        all_bi.append(new_all_baseflow);
//    }

//        SampleInfo new_samples;
//        for (int i=0;i<10;i++)
//        {
//            new_samples.setU1(rand()%10);
//            new_samples.setU2(rand()%10);
//            new_samples.setLocationId(i%3 + 1);
//            new_samples.setWaterId(Names::analitic_id);
//            new_samples.setDate(QDateTime::fromString("2011-11-11 11:11", "yyyy-MM-dd hh:mm"));
//            si.append(new_samples);
//        }

//        SampleInfo new_baseflow;
//        for (int i=0;i<10;i++)
//        {
//            new_baseflow.setU1(rand()%10);
//            new_baseflow.setU2(rand()%10);
//            new_baseflow.setLocationId(i%3 + 1);
//            new_baseflow.setWaterId(i%6 + 1);
//            new_baseflow.setDate(QDateTime::fromString("2014-05-12 00:12", "yyyy-MM-dd hh:mm"));
//            bi.append(new_baseflow);
//        }
//    one.setSamplesX(si);
//    one.setSamplesY(si);
    one.setBaseflowX(bi);
    one.setBaseflowY(bi);
    plot->addAllBaseflow(all_bi);  //В график добавляем все источники
    one.generateSets(bi, bi);
    plot->setData(one);

}
