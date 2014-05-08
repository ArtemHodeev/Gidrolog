#include "itemtypecombobox.h"
#include <QDebug>
#include <names.h>
#include <QHash>
#include <QComboBox>

Itemtypecombobox::Itemtypecombobox(QObject *parent): ComboboxDelegate(parent)
{

}
Itemtypecombobox::~Itemtypecombobox()
{

}
QWidget* Itemtypecombobox::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox* editor = new QComboBox(parent);
    QHash<QString,unsigned int>::iterator i;

    // Установка значений выпадающего списка
    for (i = Names::item_types->begin(); i != Names::item_types->end(); i++)
    {
        editor->addItem(i.key());
    }
    return editor;
}
