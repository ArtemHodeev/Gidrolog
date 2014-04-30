#ifndef CONFIRMWATERTYPEMODEL_H
#define CONFIRMWATERTYPEMODEL_H
#include <watertypemodel.h>
#include <watertype.h>

class ConfirmWaterTypeModel : public WaterTypeModel
{
    Q_OBJECT
public:
    explicit ConfirmWaterTypeModel(QObject * parent = 0);
    ~ConfirmWaterTypeModel();
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    void setItems(QVector<WaterType*> new_items);
    void saveItems();
};

#endif // CONFIRMWATERTYPEMODEL_H
