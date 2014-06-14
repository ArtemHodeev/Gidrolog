#include "doubleboxdelegate.h"
#include <QDoubleSpinBox>

DoubleBoxDelegate::DoubleBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}
QWidget *DoubleBoxDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(0.0);
    editor->setMaximum(100.0);
    return editor;
}
void DoubleBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    double value = index.model()->data(index, Qt::EditRole).toDouble();
    QDoubleSpinBox *doublespinBox = static_cast<QDoubleSpinBox*>(editor);
    doublespinBox->setValue(value);
}
void DoubleBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QDoubleSpinBox *doublespinBox = static_cast<QDoubleSpinBox*>(editor);
    doublespinBox->interpretText();
    double value = doublespinBox->value();
    model->setData(index, value, Qt::EditRole);
}
void DoubleBoxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
