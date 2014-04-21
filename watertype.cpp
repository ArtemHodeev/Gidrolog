#include "watertype.h"

WaterType::WaterType()
{
    id  = 0;
    name = "";
    position = 0;
}
WaterType::WaterType(const WaterType &other)
{
    id = other.getId();
    name = other.getName();
    position = other.getPosition();
}
WaterType::~WaterType()
{
}

unsigned int WaterType::getId() const
{
    return id;
}
unsigned int WaterType::getPosition() const
{
    return position;
}
QString WaterType::getName() const
{
    return name;
}
void WaterType::setId(unsigned int id)
{
    this->id = id;
}
void WaterType::setPosition(unsigned int position)
{
    this->position = position;
}
void WaterType::setName(QString name)
{
    this->name = name;
}
