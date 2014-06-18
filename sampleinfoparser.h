#ifndef SAMPLEINFOPARSER_H
#define SAMPLEINFOPARSER_H
#include <QObject>
#include <sampleinfo.h>

class SampleInfoParser : public QObject
{
    Q_OBJECT
public:
    explicit SampleInfoParser(QObject *parent = 0);
    QString getWaterTypeName(unsigned int water_id) const;
    double getAveU1(QVector<SampleInfo>) const;
    double getAveU2(QVector<SampleInfo>) const;

signals:

public slots:
private:
    double ave_U1;
    double ave_U2;
    //QVector <SampleInfo> samples;
    QString water_type;
};

#endif // SAMPLEINFOPARSER_H
