#include "locationcombobox.h"
#include <names.h>
#include <QHash>
#include <QComboBox>

LocationCombobox::LocationCombobox(QObject *parent):
    ComboboxDelegate(parent)
{
}
LocationCombobox::~LocationCombobox()
{}

/*
 *F:
 *   QWidget* LocationCombobox::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
 * I:
 *  QWidget *parent
 *  const QStyleOptionViewItem &option
 *  const QModelIndex &index - индекс элемента модели, для которого применяется делегат
 * O:
 *  QWidget* - указатель на виджет, который будет отображаться на экране
 * D:
 *  Функция устанавливает внешний вид делегата как combobox и задает значения выпадающего списка
 */
QWidget* LocationCombobox::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox* editor = new QComboBox(parent);
    QHash<QString,unsigned int>::iterator i;

    // Установка значений выпадающего списка
    for (i = Names::locations->begin(); i != Names::locations->end(); i++)
    {
        editor->addItem(i.key());
    }
    return editor;
}
