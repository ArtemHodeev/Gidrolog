#include "iteminfo.h"

ItemInfo::ItemInfo()
{
    item_id = 0;
    lost_count = 0;
    error_count = 0;
    position = -1;
}

ItemInfo::ItemInfo(const ItemInfo &other)
{
    item_id = other.getItemId();
    lost_count = other.getLostCount();
    error_count = other.getErrorCount();
    position = other.getPosition();
    correlations = other.getCorrelations();
}

ItemInfo::~ItemInfo()
{
}

void ItemInfo::setItemId(unsigned int item_id)
{
    this->item_id = item_id;
}

void ItemInfo::setLostCount(float lost_count)
{
    this->lost_count = lost_count;
}

void ItemInfo::setErrorCount(float error_cout)
{
    this->error_count = error_cout;
}

void ItemInfo::setPosition(unsigned int position)
{
    this->position = position;
}

void ItemInfo::setCorrelations(QVector<ItemCorellation *> corrls)
{
    ItemCorellation *i_c;
    for (int i = 0; i < corrls.size(); i ++)
    {
        i_c = new ItemCorellation();
        i_c->setItemId(corrls[i]->getItemId());
        i_c->setCorell(corrls[i]->getCorell());
        correlations.append(i_c);
    }
}
unsigned int ItemInfo::getItemId() const
{
    return item_id;
}

float ItemInfo::getLostCount() const
{
    return lost_count;
}

float ItemInfo::getErrorCount() const
{
    return error_count;
}

unsigned int ItemInfo::getPosition() const
{
    return position;
}

QVector<ItemCorellation*> ItemInfo::getCorrelations() const
{
    return correlations;
}

