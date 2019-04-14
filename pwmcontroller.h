#ifndef PWMCONTROLLER_H
#define PWMCONTROLLER_H

#include <QObject>

#include "pwmdata.h"
#include "pwmconnection.h"

class pwmController : public QObject
{
    Q_OBJECT
public:
    pwmController(pwmData& aPwmData, pwmConnection& aPwmConnection);// pwmConnection& aPwmConnection);

public slots:
    void SetPwmOn(bool on);

    void UpdateFrequency(double frequency);
    void UpdateDutyCycle(double dutyCycle);

private:

    void connectSignalsAndSlots(void);
    double convertFrequency(double frequency);

    pwmData& m_pwmData;
    pwmConnection& m_pwmConnection;
};

#endif // PWMCONTROLLER_H
