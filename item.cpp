#include "item.h"
#include <QDebug>
Item::Item()
{
    id = 0;
    name = "";
    type_id = 0;
    min_value = 0;
    error_line = 0;
}
unsigned int Item::getId() const
{
    return id;
}
QString Item::getName() const
{
    return name;
}
unsigned int Item::getTypeId() const
{
    return type_id;
}
double Item::getMinValue() const
{
    return min_value;
}
double Item::getErrorLine() const
{
    return error_line;
}
void Item::setId(const unsigned int &id)
{
    this->id = id;
}
void Item::setName(const QString &name)
{
    this->name = name;
}
void Item::setTypeId(const unsigned int &typeId)
{
    this->type_id = typeId;
}
void Item::setMinValue(const double &minValue)
{
    this->min_value = minValue;
}
void Item::setErrorLine(const double &errorLine)
{
    this->error_line = errorLine;
}



