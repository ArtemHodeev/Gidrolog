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
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QHash<QString, unsigned int> *getParams();
    QHash<QString, unsigned int> *getWaterTypes();
    QHash<QString, unsigned int> *getLocations();
    void setHeaders();
    void resetModel(QVector<Sample *> sample_mass);
    void resetModel();
    void setWaterTypes();
    void setLocation();
    void setAnaliticId();
    void setSamples(QVector<Sample*> sample_mass);
    QVector<Sample*> getSample();
    int findItemInPosition(unsigned int pos);
    void setItems();
    void setItemsToDelete(int *mass);
    void deleteAllSamples();
    void saveItems();
    void updateItems();
    void removeItems();

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
    QHash<QString, unsigned int> *water_types; // все типы водных масс из БД
//                                                QSqtring - имя типа водной массы, unsigned int - id из БД
    QHash<QString, unsigned int> *locations;
    QVector<QString> headers; // заголовки столбцов

};

#endif // SAMPLEMODEL_H
