#ifndef ITEMINFO_H
#define ITEMINFO_H
#include <itemcorellation.h>
#include <QVector>
class ItemInfo
{
public:
    ItemInfo();
    ItemInfo(const ItemInfo &other);
    ~ItemInfo();
    void setItemId(unsigned int item_id);
    void setLostCount(float lost_count);
    void setErrorCount(float error_cout);
    void setPosition(unsigned int position);
    void setCorrelations(QVector<ItemCorellation*> corrls);
    unsigned int getItemId() const;
    float getLostCount() const;
    float getErrorCount() const;
    unsigned int getPosition() const;
    QVector<ItemCorellation*> getCorrelations() const;

private:
    unsigned int item_id;
    float lost_count;
    float error_count;
    QVector<ItemCorellation*> correlations;
    unsigned int position;
};

#endif // ITEMINFO_H
