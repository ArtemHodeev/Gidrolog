#include "standartization.h"
#include <names.h>

Standartization::Standartization()
{
    analitic_average = 0;
}
double Standartization::getAverage(double *mass, int length)
{
    double sum = 0;
    for (int i = 0; i < length; i ++)
    {
        sum += mass[i];
    }
    return sum / length;
}
double Standartization::getAverageSquare(double *mass_1, double average_1, double *mass_2, double average_2, int length)
{
    double sum = 0;
    for (int i = 0; i < length; i ++)
    {
        sum += (mass_1[i] - average_1) * (mass_2[i] - average_2);
    }

    return sum / length;
}

