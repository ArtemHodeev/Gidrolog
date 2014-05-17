#include "checkboxdelegate.h"
#include <QCheckBox>
#include <QVariant>
#include <QMetaType>

CheckBoxDelegate::CheckBoxDelegate(QObject *parent):
    QItemDelegate(parent)
{
}
CheckBoxDelegate::~CheckBoxDelegate()
{}
QWidget* CheckBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QCheckBox *box = new QCheckBox(parent);
    return box;
}
void CheckBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QCheckBox *box = static_cast<QCheckBox*>(editor);
    box->setChecked(index.data().toBool() | true);
}
void CheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QCheckBox *box = static_cast<QCheckBox*>(editor);
    model->setData(index, index.data().toBool() | true, Qt::EditRole);
}
void CheckBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
//void CheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//    if (qVariantCanConvert<QCheckBox>(index.data()))
//    {
//        QCheckBox *box = static_cast<QCheckBox*>(index.data());
//    painter
//        box->paint(painter, option.rect, option.palette,
//                   StarRating::ReadOnly);
//    }
//    else
//    {
//        QItemDelegate::paint(painter,option,index);
//    }
//}
void CheckBoxDelegate::drawCheck(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, Qt::CheckState state)
{

}
