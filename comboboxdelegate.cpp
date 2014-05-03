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

/*
 *F:
 *   void ComboboxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
 * I:
 *  QWidget *editor
 *  const QModelIndex &index - индекс элемента модели, для которого применяется делегат
 * O:
 *  --
 * D:
 *  Функция устанавливает значение ячейки, видимоет на экране
 */
void ComboboxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = index.model()->data(index, Qt::EditRole).toString();//.toUInt();
    comboBox->setCurrentText(value);
}

/*
 *F:
 *   void ComboboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
 * I:
 *  QWidget *editor
 *  QAbstractItemModel *model
 *  const QModelIndex &index - индекс элемента модели, для которого применяется делегат
 * O:
 *  --
 * D:
 *  Функция устанавливает значение в моделе данных
 */
void ComboboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    model->setData(index,comboBox->currentText()/*->currentIndex()*/, Qt::EditRole);
}

/*
 *F:
 *   void ComboboxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex & index ) const
 * I:
 *  QWidget *editor
 *  const QStyleOptionViewItem &option
 *  const QModelIndex &index - индекс элемента модели, для которого применяется делегат
 * O:
 *  --
 * D:
 *  Функция устанавливает внешний вид делегата
 */
void ComboboxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
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
