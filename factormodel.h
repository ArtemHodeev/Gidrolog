#ifndef FACTORMODEL_H
#define FACTORMODEL_H
#include <QStringList>
class FactorModel
{
public:
    FactorModel();
    ~FactorModel();
    void setItems();
    void saveItems();
    void setCorell(const float &cor);
    void setLostCount(const float &lost);
    void setErrorCount(const float &error);
    float getLostCount();
    float getErrorCount();
    float getCorell();
    QStringList getWaterTypes();
    unsigned int getAnaliticId();
    QString getAnaliticName();
    void setAnaliticId(const QString &name);
private:
    float lost_count; // максимально допустимы процент пропусков данных
    float error_count; // максимально допустимы процент ошибочных данных
    float corell; // максимально допустимая корелляция
    unsigned int analitic_id; // id анализируемого типа водной массы
};

#endif // FACTORMODEL_H
