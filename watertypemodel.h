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
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    void setItems();
//    void setItems(QVector<WaterType*> new_items);
    virtual void saveItems();
    void updateItems();
    void removeItems();
    void setItemsToDelete(int *mass);
    int findItemInPosition(unsigned int pos);
protected:
    QVector<WaterType*> items;
    QVector<WaterType*> items_to_delete;
    QVector<WaterType*> items_to_save;
    QVector<WaterType*> items_to_update;
};

#endif // WATERTYPEMODEL_H
