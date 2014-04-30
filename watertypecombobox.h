#ifndef WATERTYPECOMBOBOX_H
#define WATERTYPECOMBOBOX_H
//#include
#include <comboboxdelegate.h>

class WaterTypeCombobox : public ComboboxDelegate
{
    Q_OBJECT
public:
    explicit WaterTypeCombobox(QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // WATERTYPECOMBOBOX_H
