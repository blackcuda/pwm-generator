#ifndef PWMDATA_H
#define PWMDATA_H

#include<QObject>

class pwmData : public QObject
{
    Q_OBJECT
public:
    explicit pwmData();
    //~pwmData();

    void SetPwmOn(bool on);
    void SetFrequency(double frequency);
    void SetDutyCycle(double dutyCycle);
    void SetCounts(unsigned int counts);
    void SetFrequencyUnit(unsigned int unit);

    double GetFrequency(void);
    double GetDutyCycle(void);
    unsigned int GetCounts(void);

    bool IsPwmOn(void);

    int GetFrequencyUnit(void);

signals:

    void pwmOnChanged(bool on);
    void frequencyChanged(double frequency);
    void dutyCycleChanged(double dutyCycle);
    void countsChanged(unsigned int counts);
    void frequencyUnitChanged(unsigned int unit);

private:

    bool m_pwmOn;

    double m_frequency;
    double m_dutyCycle;
    unsigned int m_counts;

    unsigned int m_frequencyUnit;
};

#endif // PWMDATA_H
