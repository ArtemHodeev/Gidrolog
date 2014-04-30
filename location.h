#ifndef LOCATION_H
#define LOCATION_H
#include <QString>

class Location
{
public:
    Location();
    ~Location();
    unsigned int getId() const;
    QString getName() const;
    float getWidth() const;
    float getLength() const;
    float getDeep() const;
    int getPosition() const;
    void setId(unsigned int id);
    void setName(QString name);
    void setWidth(float width);
    void setLength(float length);
    void setDeep(float deep);
    void setPosition(int position);
private:
    unsigned int id;
    QString name;
    float width;
    float length;
    float deep;
    int position;
};

#endif // LOCATION_H
