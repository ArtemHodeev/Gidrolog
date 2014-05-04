#include "tablemodel.h"
#include <QDebug>
TableModel::TableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    rCount = 0;
    cCount = 0;
}
TableModel::~TableModel()
{
    qDebug()<<"Destructor of TableModel";
}
int TableModel::rowCount(const QModelIndex &parent) const
{
    return rCount;
}
int TableModel::columnCount(const QModelIndex &parent) const
{
    return cCount;
}
/*
 * F:
 *  bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
 * I:
 *  row - номер строки перед, которой необходимо вставить строку
 *  count - количество вставляемы строк
 *  &parent - ссылка на родителя, строк (по умолчанию пусто)
 * O:
 *  true - строки добавлены
 *  false - либо клличество строк, либо номер строки,
 *   перед которой нужно вставить строки, отрицательны. строки не вставлены
 * D:
 *  Функция вставляет count строк перед строкой row, строки перенумеровываются
 */
bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || count < 0)
    {
        return false;
    }

    // вставка строк: вставить одну строку с номера row -1, по номер row -1
    beginInsertRows(QModelIndex(), row - 1, row - 1);
    rCount += count;
    endInsertRows();

    return true;
}
bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || count < 0)
    {
        return false;
    }

    beginRemoveRows(parent, row, row + count - 1);
    rCount -= count;
    endRemoveRows();
}

void TableModel::setItemsToDelete(int *mass)
{
}
int TableModel::findItemInPosition(unsigned int pos)
{
    return -1;
}

