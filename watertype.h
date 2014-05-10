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
    int getPosition() const;
    QString getName() const;
    void setId(unsigned int id);
    void setPosition(int position);
    void setName(QString name);
private:
    unsigned int id;
    QString name;
    int position;

};

#endif // WATERTYPE_H
