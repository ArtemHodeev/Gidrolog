#ifndef ItemModel_H
#define ItemModel_H

#include <QAbstractTableModel>
#include <item.h>
#include <QCloseEvent>
#include <tablemodel.h>

class ItemModel : public TableModel
{
    Q_OBJECT
public:
    explicit ItemModel(QObject *parent = 0);
    ~ItemModel();
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    int findItemInPosition(unsigned int pos);
    void setItems();
    void setItemsToDelete(unsigned int *mass);
    void saveItems();
    void updateItems();
    void removeItems();


private:
    QVector<Item*> items_to_update;
    QVector<Item*> items_to_delete;
    QVector<Item*> items_to_save;
    QVector<Item*> items; // список всех компонентов из БД

signals:

public slots:
//    void actionSave_clicked();
//    void on_saveButton_clicked();
//    void on_pushButton_exit_clicked();
//    void on_removeButton_clicked();
};

#endif // ItemModel_H
