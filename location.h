#ifndef LOCATION_H
#define LOCATION_H
#include <QString>

class Location
{
public:
    Location();
    Location(const Location &other);
    ~Location();
    unsigned int getId() const;
    QString getName() const;
    double getWidth() const;
    double getLength() const;
    double getDeep() const;
    int getPosition() const;
    void setId(unsigned int id);
    void setName(QString name);
    void setWidth(double width);
    void setLength(double length);
    void setDeep(double deep);
    void setPosition(int position);
private:
    unsigned int id;
    QString name;
    double width;
    double length;
    double deep;
    int position;
};

#endif // LOCATION_H
