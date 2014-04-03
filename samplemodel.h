#ifndef SAMPLEMODEL_H
#define SAMPLEMODEL_H

#include <QAbstractTableModel>
#include <tablemodel.h>
#include <sample.h>

class SampleModel : public TableModel
{
    Q_OBJECT
public:
    explicit SampleModel(QObject *parent = 0);
    ~SampleModel();
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
//    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
//    QVariant getName(unsigned int key);
//    void setParams();
    unsigned int getVaule(QString param_name) const;
    void setItems();
    void saveItems();
    void setHeaders();


private:
    QVector<Sample*> items_to_save;
    QVector<Sample*> items;
    QHash<QString, unsigned int> *params;
    QVector<QString> headers; // параметры: QString - имя параметра, unsigned int - id параметра

};

#endif // SAMPLEMODEL_H
