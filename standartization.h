#ifndef STANDARTIZATION_H
#define STANDARTIZATION_H

class Standartization
{
public:
    double analitic_average;

    Standartization();

    virtual double getAverage(double *mass, int length);
    virtual double getAverageSquare(double *mass_1, double average_1, double *mass_2, double average_2, int length);

};

#endif // STANDARTIZATION_H
