#ifndef ITEMTABLEMODEL_H
#define ITEMTABLEMODEL_H

#include <QAbstractTableModel>
#include <item.h>
#include <QCloseEvent>
#include <tablemodel.h>

class ItemTableModel : public TableModel
{
    Q_OBJECT
public:
    explicit ItemTableModel(QObject *parent = 0);
    ~ItemTableModel();
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void setItems();
    void saveItems();
    void removeItems();

    QList<Item*> items_to_save;// список измененных компонентов, которые необходимо сохранить в БД
    QList<Item*> items_to_remove;
private:
    QList<Item*> items; // список всех компонентов из БД
signals:

public slots:
    void actionSave_clicked();
    void on_saveButton_clicked();
    void on_pushButton_exit_clicked();
    void on_removeButton_clicked();
};

#endif // ITEMTABLEMODEL_H
