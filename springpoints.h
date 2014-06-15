#ifndef SPRINGPOINTS_H
#define SPRINGPOINTS_H
#include <analiticpoints.h>

class SpringPoints : public AnaliticPoints
{
    double average_u_1;
    double average_u_2;
public:
    SpringPoints();
    ~SpringPoints();
    void setAverageU1(double average);
    void setAverageU2(double average);
    double getAverageU1() const;
    double getAverageU2() const;
};

#endif // SPRINGPOINTS_H
