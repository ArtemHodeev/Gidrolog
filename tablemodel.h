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
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

private:

signals:

public slots:
};

#endif // TABLEMODEL_H
