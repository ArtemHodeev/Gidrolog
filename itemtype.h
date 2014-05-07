#ifndef ITEMTYPE_H
#define ITEMTYPE_H
#include <QString>
class ItemType
{
    unsigned int id;
    unsigned int priority;
    QString name;
    int position;

public:
    ItemType();
    ItemType(const ItemType &other);

    ~ItemType();

    unsigned int getId() const;
    void setId(const unsigned int &id);

    unsigned int getPriority() const;
    void setPriority(const unsigned int &priority);

    QString getName() const;
    void setName(const QString &name);

    int getPosition() const;
    void setPosition(int position);
};

#endif // ITEMTYPE_H
