#include "watertypecombobox.h"
#include <names.h>
#include <QDebug>
#include <QComboBox>

WaterTypeCombobox::WaterTypeCombobox(QObject *parent):
    ComboboxDelegate(parent)
{
}

QWidget* WaterTypeCombobox::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug()<<"createEditor";
    QComboBox* editor = new QComboBox(parent);
    QHash<QString,unsigned int>::iterator i;
    qDebug()<<"Items: "<<Names::water_types;
    qDebug()<<"Items size: "<<Names::water_types->size();
//    if (index)
    for (i = Names::water_types->begin(); i != Names::water_types->end(); i++)
    {
        editor->addItem(i.key());
    }
    return editor;
}
