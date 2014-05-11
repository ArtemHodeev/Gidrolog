#include "itemcorellation.h"

ItemCorellation::ItemCorellation()
{
    item_id = 0;
    corell = 0;
}
ItemCorellation::ItemCorellation(const ItemCorellation &other)
{
    item_id = other.getItemId();
    corell = other.getCorell();
}

void ItemCorellation::setItemId(unsigned int item_id)
{
    this->item_id = item_id;
}
void ItemCorellation::setCorell(double corell)
{
    this->corell = corell;
}
unsigned int ItemCorellation::getItemId() const
{
    return item_id;
}
double ItemCorellation::getCorell() const
{
    return corell;
}
