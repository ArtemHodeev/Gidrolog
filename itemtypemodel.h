#ifndef ITEMTYPEMODEL_H
#define ITEMTYPEMODEL_H
#include <itemtype.h>
#include <tablemodel.h>

class ItemTypeModel : public TableModel
{
    Q_OBJECT
public:
    explicit ItemTypeModel(QObject *parent = 0);
    ~ItemTypeModel();
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void setItems();
    void saveItems();
    void updateItems();
    void removeItems();

    QList<ItemType*> items_to_save;
private:
    QList<ItemType*> items;
};

#endif // ITEMTYPEMODEL_H
