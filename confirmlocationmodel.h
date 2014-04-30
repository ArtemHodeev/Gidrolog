#ifndef CONFIRMLOCATIONMODEL_H
#define CONFIRMLOCATIONMODEL_H
#include <locationmodel.h>

class ConfirmLocationModel : public LocationModel
{
    Q_OBJECT
public:
    explicit ConfirmLocationModel(QObject *parent = 0);
    ~ConfirmLocationModel();
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setItems(QVector<Location*> new_items);
    void saveItems();

signals:

public slots:

};

#endif // CONFIRMLOCATIONMODEL_H
