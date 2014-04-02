#ifndef ITEMTYPE_H
#define ITEMTYPE_H
#include <QString>
class ItemType
{
    unsigned int id;
    unsigned int priority;
    QString name;

public:
    ItemType();
    unsigned int getId() const;
    unsigned int getPriority() const;
    QString getName() const;
    void setId(const unsigned int &id);
    void setPriority(const unsigned int &priority);
    void setName(const QString &name);
};

#endif // ITEMTYPE_H
