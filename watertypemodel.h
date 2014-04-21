#ifndef WATERTYPEMODEL_H
#define WATERTYPEMODEL_H
#include <tablemodel.h>
#include <QVariant>
#include <QVector>
#include <watertype.h>

class WaterTypeModel : public TableModel
{
public:
    explicit WaterTypeModel(QObject *parent = 0);
    ~WaterTypeModel();
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setItems();
    void saveItems();
    void updateItems();
    void removeItems();
    void setItemsToDelete(int *mass);
private:
    QVector<WaterType*> items;
    QVector<WaterType*> items_to_delete;
    QVector<WaterType*> items_to_save;
    QVector<WaterType*> items_to_update;
};

#endif // WATERTYPEMODEL_H
