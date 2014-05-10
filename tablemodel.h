#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

protected:
    int rCount; // количество строк таблицы
    int cCount; // количество столбцов таблицы

public:
    explicit TableModel(QObject *parent = 0);
    ~TableModel();
    virtual void setItems() = 0;
    virtual void saveItems() = 0;
    virtual void updateItems() = 0;
    virtual void removeItems() = 0;
    virtual void setItemsToDelete(int *mass);
    virtual int findItemInPosition(int pos);
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeColumns(int column, int count, const QModelIndex &parent);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
private:

signals:
//    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
public slots:
};

#endif // TABLEMODEL_H
