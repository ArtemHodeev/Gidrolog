#ifndef ITEMINSAMPLE_H
#define ITEMINSAMPLE_H

class ItemInSample
{
public:
    ItemInSample();
    ItemInSample(const ItemInSample &other);
    int getChanged() const;
    unsigned int getItemId() const;
    double getValue() const;
    void setChanged(int changed);
    void setItemId(unsigned int item_id);
    void setValue(double value);
private:
    int changed;
    unsigned int item_id;
    double value;
};

#endif // ITEMINSAMPLE_H
