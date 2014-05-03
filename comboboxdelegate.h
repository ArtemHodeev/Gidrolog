#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QAbstractItemDelegate>
#include <QItemDelegate>
//#include <QHash>

class ComboboxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ComboboxDelegate(QObject *parent = 0);
    ~ComboboxDelegate();
//    void setItems(QHash<QString, unsigned int> *new_params);
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
//      void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:

signals:

public slots:

};

#endif // COMBOBOXDELEGATE_H
