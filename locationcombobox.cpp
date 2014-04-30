#include "locationcombobox.h"
#include <names.h>
#include <QHash>
#include <QComboBox>

LocationCombobox::LocationCombobox(QObject *parent):
    ComboboxDelegate(parent)
{
}

QWidget* LocationCombobox::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    qDebug()<<"createEditor";
    QComboBox* editor = new QComboBox(parent);
    QHash<QString,unsigned int>::iterator i;
//    qDebug()<<"Items: "<<Names::water_types;
//    qDebug()<<"Items size: "<<Names::water_types->size();
//    if (index)
    for (i = Names::locations->begin(); i != Names::locations->end(); i++)
    {
        editor->addItem(i.key());
    }
    return editor;
}
