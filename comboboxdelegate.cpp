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
    items = new QHash<QString,unsigned int>();
//    locations = new QHash<QString,unsigned int>();
//    Items.push_back("Test0");
//      Items.push_back("Test1");
//      Items.push_back("Test2");
//      Items.push_back("Test3");
//      Items.push_back("Test4");
//      Items.push_back("Test5");
//      Items.push_back("Test6");
//      Items.push_back("Test7");
//      Items.push_back("Test8");
}
void ComboboxDelegate::setItems(QHash<QString,unsigned int> *new_params)
{
    QHash<QString,unsigned int>::iterator i;
    for (i = new_params->begin(); i != new_params->end(); i++)
    {
        items->insert(i.key(),i.value());
    }
}

ComboboxDelegate::~ComboboxDelegate()
{
    delete items;
}
QWidget *ComboboxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex & index ) const
{
    qDebug()<<"createEditor";
    QComboBox* editor = new QComboBox(parent);
    QHash<QString,unsigned int>::iterator i;
    for (i = items->begin(); i != items->end(); i++)
    {
        editor->addItem(i.key());
    }
    return editor;
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
    model->setData(index, comboBox->currentText()/*->currentIndex()*/, Qt::EditRole);
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
