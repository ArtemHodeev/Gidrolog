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
    int position;
    bool display;

public:
    Item();
    ~Item();
    void setId(const unsigned int &id);
    void setName(const QString &name);
    void setTypeId(const unsigned int &typeId);
    void setMinValue(const double &minValue);
    void setErrorLine(const double &errorLine);
    void setPosition(const double &position);
    void setDisplay(bool display);
    unsigned int getId() const;
    unsigned int getPosition() const;
    unsigned int getTypeId() const;
    QString getName() const;
    double getMinValue() const;
    double getErrorLine() const;
    bool getDisplay() const;
};

#endif // ITEM_H
