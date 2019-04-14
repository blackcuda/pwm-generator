#ifndef PWMCONNECTION_H
#define PWMCONNECTION_H

#include <QObject>

#include "pwm.h"
#include "pwmdata.h"

class pwmConnection : public QObject
{
    Q_OBJECT
public:
    pwmConnection(PWM& aPwm, pwmData& aPwmData);

    void SetPwmOn();
    void SetPwmOff();

public slots:
    void SetFrequency(double frequency);
    void SetDutyCycle(double dutyCycle);
    void SetCounts(unsigned int counts);

private:
    void connectSignalsAndSlots();
    void disconnectSignalsFromSlots();

    PWM& m_pwm;
    pwmData& m_pwmData;
};

#endif // PWMCONNECTION_H
