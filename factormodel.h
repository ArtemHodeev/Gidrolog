#ifndef FACTORMODEL_H
#define FACTORMODEL_H

class FactorModel
{
public:
    FactorModel();
    ~FactorModel();
    void setItems();
    void saveItems();
private:
    float lost_count; // максимально допустимы процент пропусков данных
    float error_count; // максимально допустимы процент ошибочных данных
    float corell; // максимально допустимая корелляция
    unsigned int analitic_id; // id анализируемого типа водной массы
};

#endif // FACTORMODEL_H
