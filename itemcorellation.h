#ifndef ITEMCORELLATION_H
#define ITEMCORELLATION_H

class ItemCorellation
{
    unsigned int item_id;
    double corell;
public:
    ItemCorellation();
    ItemCorellation(const ItemCorellation &other);
    void setItemId(unsigned int item_id);
    void setCorell(double corell);
    unsigned int getItemId() const;
    double getCorell() const;
};

#endif // ITEMCORELLATION_H
