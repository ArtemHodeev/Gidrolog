#ifndef ITEMINFO_H
#define ITEMINFO_H

class ItemInfo
{
public:
    ItemInfo();
    ItemInfo(const ItemInfo &other);
    ~ItemInfo();
    void setItemId(unsigned int item_id);
    void setLostCount(int lost_count);
    void setErrorCount(int error_cout);
    unsigned int getItemId() const;
    int getLostCount() const;
    int getErrorCount() const;

private:
    unsigned int item_id;
    int lost_count;
    int error_count;
};

#endif // ITEMINFO_H
