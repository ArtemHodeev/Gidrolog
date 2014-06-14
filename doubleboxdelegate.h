#ifndef DOUBLEBOXDELEGATE_H
#define DOUBLEBOXDELEGATE_H
#include <QStyledItemDelegate>

class DoubleBoxDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    DoubleBoxDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
#endif // DOUBLEBOXDELEGATE_H
