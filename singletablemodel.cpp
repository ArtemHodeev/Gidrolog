#include "singletablemodel.h"

SingleTableModel::SingleTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}
QVariant SingleTableModel::data(const QModelIndex &index, int role) const
{

}
int SingleTableModel::rowCount(const QModelIndex &parent) const
{}
int SingleTableModel::columnCount(const QModelIndex &parent) const
{}
QVariant SingleTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{}
Qt::ItemFlags SingleTableModel::flags(const QModelIndex &index) const
{}
bool SingleTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{}
bool SingleTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{}
void SingleTableModel::setQuery()
{
    sql = QString("SELECT :table.* FROM :table").arg("table", table);
}
void SingleTableModel::setQuery(QString display_str, QString join_str)
{
    QStringList mass = sql.split("FROM");
    mass[0] += QString(", :display_str ").arg("display_str", display_str);
    mass[1] += QString(" :join_str").arg("join_str",join_str);
    sql = mass[0] + mass[1];
}
void SingleTableModel::setTable(QString table)
{
    this->table = table;
    setQuery();
}
void SingleTableModel::setRelation(QString loc_field, QString rel_table, QString rel_field, QString display_field)
{
    QString display_str = QString(":rel_table.:display_name")
            .arg("rel_table",rel_table)
            .arg("display_name",display_field);

    QString join_str = QString("JOIN :rel_table ON :table.:loc_field = :rel_table.rel_field")
            .arg("table",table)
            .arg("rel_table", rel_table)
            .arg("loc_field", loc_field)
            .arg("rel_field",rel_field);

    setQuery(display_str, join_str);
}
