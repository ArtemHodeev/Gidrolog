#ifndef ITEMTYPECOMBOBOX_H
#define ITEMTYPECOMBOBOX_H
#include <comboboxdelegate.h>

class Itemtypecombobox : public ComboboxDelegate
{
    Q_OBJECT
public:
    explicit Itemtypecombobox(QObject *parent = 0);
    ~Itemtypecombobox();
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // ITEMTYPECOMBOBOX_H
