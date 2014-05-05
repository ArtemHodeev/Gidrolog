#ifndef CONFIRMCALCULATORMODEL_H
#define CONFIRMCALCULATORMODEL_H
#include <tablemodel.h>
#include <iteminfo.h>

class ConfirmCalculatorModel : public TableModel
{
    Q_OBJECT
public:
    explicit ConfirmCalculatorModel(QObject *parent = 0);
    ~ConfirmCalculatorModel();
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void saveItems();
    void updateItems();
    void removeItems();
    void setItems();
    void setItems(QVector<ItemInfo*> other);
    void setParams();
    int findItemById(const unsigned int &item_id) const;
private:
    QVector<ItemInfo*> items; // все параметры с их информацией о прохождении метода грубого отсева
    QHash<unsigned int, unsigned int> *params; // хэш, содержащий пары <номер столбца, id параметра>
    QVector<ItemInfo*> items_to_delete; // параметры которые следует убрать из обработки и не отображать на экране

signals:

public slots:

};

#endif // CONFIRMCALCULATORMODEL_H
