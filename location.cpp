#include "location.h"

Location::Location()
{
    id = 0;
    name = "";
    width = -1;
    length = -1;
    deep = -1;
    position = -1;
}
Location::~Location()
{
}

unsigned int Location::getId() const
{
    return id;
}

QString Location::getName() const
{
    return name;
}
float Location::getWidth() const
{
    return width;
}
float Location::getLength() const
{
    return length;
}
float Location::getDeep() const
{
    return deep;
}
int Location::getPosition() const
{
    return position;
}
void Location::setId(unsigned int id)
{
    this->id = id;
}
void Location::setName(QString name)
{
    this->name = name;
}
void Location::setWidth(float width)
{
    this->width = width;
}
void Location::setLength(float length)
{
    this->length = length;
}
void Location::setDeep(float deep)
{
    this->deep = deep;
}
void Location::setPosition(int position)
{
    this->position = position;
}
