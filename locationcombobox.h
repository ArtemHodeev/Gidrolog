#ifndef LOCATIONCOMBOBOX_H
#define LOCATIONCOMBOBOX_H
#include <comboboxdelegate.h>

class LocationCombobox : public ComboboxDelegate
{
    Q_OBJECT
public:
    explicit LocationCombobox(QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // LOCATIONCOMBOBOX_H
