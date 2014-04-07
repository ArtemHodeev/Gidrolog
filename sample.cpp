#include "sample.h"

Sample::Sample()
{
    id = 0;
    comment = "";
    location_id = 0;
    position = 0;

    components = new QHash<unsigned int, ItemInSample>();
}
Sample::~Sample()
{
    delete components;
}
unsigned int Sample::getId()
{
    return id;
}
unsigned int Sample::getSampleSetId()
{
    return sample_set_id;
}
unsigned int Sample::getPosition()
{
    return position;
}
unsigned int Sample::getLocationId()
{
    return location_id;
}

QDate Sample::getDate()
{
    return date;
}
QHash<unsigned int,ItemInSample>* Sample::getComponents()
{
    return components;
}
QString Sample::getComment()
{
    return comment;
}
void Sample::setId(unsigned int id)
{
    this->id = id;
}
void Sample::setSampleSetId(unsigned int id)
{
    this->sample_set_id = id;
}
void Sample::setPosition(unsigned int position)
{
    this->position = position;
}
void Sample::setLocationId(unsigned int location_id)
{
    this->location_id = location_id;
}
void Sample::setDate(QDate date)
{
    this->date = date;
}
void Sample::setComment(QString comment)
{
    this->comment = comment;
}
void Sample::setComponents(QHash<unsigned int, ItemInSample> *components)
{
    *(this->components) = *components;
}
