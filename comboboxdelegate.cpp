#include "ComboboxDelegate.h"
#include <QComboBox>
#include <QWidget>
#include <QModelIndex>
#include <QApplication>
#include <QString>
#include <QDebug>

ComboboxDelegate::ComboboxDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}
ComboboxDelegate::~ComboboxDelegate()
{
}

void ComboboxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    qDebug()<<"setEditorData";
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = index.model()->data(index, Qt::EditRole).toString();//.toUInt();
    comboBox->setCurrentText(value);
}

void ComboboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    qDebug()<<"setModelData";

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    qDebug()<<"currentText: "<< comboBox->currentText();
    model->setData(index,comboBox->currentText()/*->currentIndex()*/, Qt::EditRole);
}

void ComboboxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    qDebug()<<"updateEditorGeometry";
    editor->setGeometry(option.rect);
}

//void ComboboxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//  QStyleOptionViewItemV4 myOption = option;
////  QString text = Items[index.row()].c_str();

////  myOption.text = text;
//  myOption.text = QString("Hello");

//  QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
//}
