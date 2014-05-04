#include "iteminfo.h"

ItemInfo::ItemInfo()
{
    item_id = 0;
    lost_count = 0;
    error_count = 0;
}

ItemInfo::ItemInfo(const ItemInfo &other)
{
    item_id = other.getItemId();
    lost_count = other.getLostCount();
    error_count = other.getErrorCount();
}

ItemInfo::~ItemInfo()
{
}

void ItemInfo::setItemId(unsigned int item_id)
{
    this->item_id = item_id;
}

void ItemInfo::setLostCount(int lost_count)
{
    this->lost_count = lost_count;
}

void ItemInfo::setErrorCount(int error_cout)
{
    this->error_count = error_cout;
}

unsigned int ItemInfo::getItemId() const
{
    return item_id;
}

int ItemInfo::getLostCount() const
{
    return lost_count;
}

int ItemInfo::getErrorCount() const
{
    return error_count;
}
