#ifndef ITEM_H
#define ITEM_H
#include <QString>

class Item
{
    unsigned int id;
    QString name;
    unsigned int type_id;
    double min_value;
    double error_line;

public:
    Item();


    void setId(const unsigned int &id);
    void setName(const QString &name);
    void setTypeId(const unsigned int &typeId);
    void setMinValue(const double &minValue);
    void setErrorLine(const double &errorLine);

    unsigned int getId() const;
    QString getName() const;
    unsigned int getTypeId() const;
    double getMinValue() const;
    double getErrorLine() const;
};

#endif // ITEM_H
