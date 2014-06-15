#include "springpoints.h"

SpringPoints::SpringPoints()
{
}
SpringPoints::~SpringPoints()
{}

double SpringPoints::getAverageU1() const
{
    return average_u_1;
}
double SpringPoints::getAverageU2() const
{
    return average_u_2;
}
void SpringPoints::setAverageU1(double average)
{
    average_u_1 = average;
}
void SpringPoints::setAverageU2(double average)
{
    average_u_2 = average;
}
