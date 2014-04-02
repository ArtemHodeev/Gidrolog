#include "itemtype.h"

ItemType::ItemType()
{
    id = 0;
    priority = 0;
    name = "";
}

unsigned int ItemType::getId() const
{
    return id;
}

unsigned int ItemType::getPriority() const
{
    return priority;
}
QString ItemType::getName() const
{
    return name;
}
void ItemType::setId(const unsigned int &id)
{
    this->id = id;
}
void ItemType::setPriority(const unsigned int &priority)
{
    this->priority = priority;
}
void ItemType::setName(const QString &name)
{
    this->name = name;
}
