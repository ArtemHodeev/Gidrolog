#ifndef ITEMTYPEMODEL_H
#define ITEMTYPEMODEL_H
#include <itemtype.h>
#include <tablemodel.h>
#include <QVector>

class ItemTypeModel : public TableModel
{
    Q_OBJECT
public:
    explicit ItemTypeModel(QObject *parent = 0);
    ~ItemTypeModel();
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    void setItems();
    virtual void saveItems();
    void updateItems();
    void removeItems();
    void setItemsToDelete( int *mass);
    int findItemInPosition(unsigned int pos);


//    QList<ItemType*> items_to_save;
//private:
//    QList<ItemType*> items;

protected:
    QVector<ItemType*> items;
private:
    QVector<ItemType*> items_to_save;
    QVector<ItemType*> items_to_update;
    QVector<ItemType*> items_to_delete;

};

#endif // ITEMTYPEMODEL_H
