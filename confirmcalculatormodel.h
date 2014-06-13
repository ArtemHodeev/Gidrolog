#ifndef CONFIRMCALCULATORMODEL_H
#define CONFIRMCALCULATORMODEL_H
#include <tablemodel.h>
#include <iteminfo.h>
//#include <

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
    void setItemsToDelete(int *mass);
//    void setParams();
    void setFactors();
    void backup();
    int findItemById(unsigned int &item_id) const;
    int findItemInPosition(unsigned int pos);
private:
    QVector<ItemInfo*> items; // все параметры с их информацией о прохождении метода грубого отсева
//    QHash<int, unsigned int> *params; // хэш, содержащий пары <номер столбца, id параметра>
    QVector<ItemInfo*> items_to_delete; // параметры которые следует убрать из обработки и не отображать на экране
    float corell;
    float lost_count;
    float error_count;
signals:

public slots:

};

#endif // CONFIRMCALCULATORMODEL_H
