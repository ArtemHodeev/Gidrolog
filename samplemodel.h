#ifndef SAMPLEMODEL_H
#define SAMPLEMODEL_H

#include <QAbstractTableModel>
#include <tablemodel.h>
#include <sample.h>
#include <iteminsample.h>

class SampleModel : public TableModel
{
    Q_OBJECT
public:
    explicit SampleModel(QObject *parent = 0);
    ~SampleModel();
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
//    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
//    QVariant getName(unsigned int key);
//    void setParams();
    unsigned int getVaule(QString param_name) const;
    void setItems();
    void setItemsToDelete(int *mass);
    void saveItems();
    void updateItems();
    void removeItems();
    void setHeaders();
public slots:
    void on_actionSave_triggered();


private:
    QVector<Sample*> items_to_update;
    QVector<Sample*> items_to_delete;
    QVector<Sample*> items_to_save;
    QVector<Sample*> items;
    QVector<ItemInSample*> params_to_save;// параметры проб, которые будут созданы: в пробе появилось значение данного параметр
    QVector<ItemInSample*> params_to_update;// параметры проб, котороые обновили свое значение
    QVector<ItemInSample*> params_to_delete;// параметры проб, которые будут удалены из БД: значение стало -1
    QHash<QString, unsigned int> *params;// все какие есть параметры в БДQ.
//                                          QString - имя параметра, unsigned int - id параметра из БД
    QVector<QString> headers; // заголовки столбцов

};

#endif // SAMPLEMODEL_H
