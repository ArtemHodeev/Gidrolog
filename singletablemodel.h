#ifndef SINGLETABLEMODEL_H
#define SINGLETABLEMODEL_H

#include <QAbstractTableModel>

class SingleTableModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    QString table;
    QString sql;
    QList<QString> rel_list;
public:
    explicit SingleTableModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    void setTable(QString table);
    void setRelation(QString loc_field,QString rel_table, QString rel_field, QString display_field);
    void setQuery();
    void setQuery(QString table_str, QString join_str);

signals:

public slots:

};

#endif // SINGLETABLEMODEL_H
