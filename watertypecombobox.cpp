#include "watertypecombobox.h"
#include <names.h>
#include <QDebug>
#include <QComboBox>

WaterTypeCombobox::WaterTypeCombobox(QObject *parent):
    ComboboxDelegate(parent)
{
}

/*
 *F:
 *   QWidget* WaterTypeCombobox::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
 * I:
 *  QWidget *parent
 *  const QStyleOptionViewItem &option
 *  const QModelIndex &index - индекс элемента модели, для которого применяется делегат
 * O:
 *  QWidget* - указатель на виджет, который будет отображаться на экране
 * D:
 *  Функция устанавливает внешний вид делегата как combobox и задает значения выпадающего списка
 */
QWidget* WaterTypeCombobox::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox* editor = new QComboBox(parent);
    QHash<QString,unsigned int>::iterator i;

    // Установка значений выпадающего списка
    for (i = Names::water_types->begin(); i != Names::water_types->end(); i++)
    {
        editor->addItem(i.key());
    }
    return editor;
}
