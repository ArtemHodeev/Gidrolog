#ifndef LOCATIONMODEL_H
#define LOCATIONMODEL_H
#include <tablemodel.h>
#include <QVector>
#include <location.h>

class LocationModel : public TableModel
{
    Q_OBJECT
public:
    explicit LocationModel(QObject *parent = 0);
    ~LocationModel();
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    void setItems();
    virtual void saveItems();
    void updateItems();
    void removeItems();
    void setItemsToDelete(int *mass);
    int findItemInPosition(int pos);
protected:
    QVector<Location*> items;
private:
    QVector<Location*> items_to_save;
    QVector<Location*> items_to_update;
    QVector<Location*> items_to_delete;
signals:

public slots:

};

#endif // LOCATIONMODEL_H
