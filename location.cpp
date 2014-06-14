#include "location.h"
#include <QDebug>

Location::Location()
{
    id = 0;
    name = "";
    width = 0.0;
    length = 0.0;
    deep = 0.0;
    position = -1;
}
Location::Location(const Location &other)
{
    this->id = other.getId();
    this->name = other.getName();
    this->width = other.getWidth();
    this->length = other.getLength();
    this->position = other.getPosition();
}
Location::~Location()
{
    qDebug()<<"destruct location";
}

unsigned int Location::getId() const
{
    return id;
}

QString Location::getName() const
{
    return name;
}
double Location::getWidth() const
{
    return width;
}
double Location::getLength() const
{
    return length;
}
double Location::getDeep() const
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
void Location::setWidth(double width)
{
    this->width = width;
}
void Location::setLength(double length)
{
    this->length = length;
}
void Location::setDeep(double deep)
{
    this->deep = deep;
}
void Location::setPosition(int position)
{
    this->position = position;
}
