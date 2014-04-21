#ifndef WATERTYPE_H
#define WATERTYPE_H
#include <QString>

class WaterType
{
public:
    WaterType();
    WaterType(const WaterType &other);
    ~WaterType();
    unsigned int getId() const;
    unsigned int getPosition() const;
    QString getName() const;
    void setId(unsigned int id);
    void setPosition(unsigned int position);
    void setName(QString name);
private:
    unsigned int id;
    QString name;
    unsigned int position;

};

#endif // WATERTYPE_H
