#ifndef ConfirmItemModel_H
#define ConfirmItemModel_H
#include <itemmodel.h>

class ConfirmItemModel : public ItemModel
{
    Q_OBJECT
public:
    explicit ConfirmItemModel(QObject *parent = 0);
    ~ConfirmItemModel();
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool hasEmptyType();
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setItems(QVector<Item*> new_items);
    void saveItems();
public slots:
//    void on_itemChanged();
signals:
    void on_itemChanged();
};

#endif // ConfirmItemModel_H
