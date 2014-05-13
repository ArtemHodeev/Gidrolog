#include "itemtype.h"
#include <QDebug>

ItemType::ItemType()
{
    id = 0;
    priority = 0;
    name = "";
    position = -1;
}
ItemType::ItemType(const ItemType &other)
{
    id = other.getId();
    name = other.getName();
    priority = other.getPriority();
    position = other.getPosition();
}
ItemType::~ItemType()
{
     qDebug()<<"destruct item type";
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
int ItemType::getPosition() const
{
    return position;
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
void ItemType::setPosition(int position)
{
    this->position = position;
}
