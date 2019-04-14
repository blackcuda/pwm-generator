#include "pwmconnection.h"

#include <iostream>

pwmConnection::pwmConnection(PWM& aPwm, pwmData& aPwmData)
    : m_pwm(aPwm)
    , m_pwmData(aPwmData)
{
    connectSignalsAndSlots();
}

void pwmConnection::connectSignalsAndSlots()
{
    connect(&m_pwmData, &pwmData::countsChanged, this, &pwmConnection::SetCounts);
}

void pwmConnection::disconnectSignalsFromSlots()
{
    disconnect(&m_pwmData, 0, this, 0);
}

void pwmConnection::SetPwmOn()
{
    m_pwm.StartPwm();
}

void pwmConnection::SetPwmOff()
{
    m_pwm.StopPwm();
}

void pwmConnection::SetFrequency(double frequency)
{
    m_pwm.SetFrequency(frequency);
}

void pwmConnection::SetDutyCycle(double dutyCycle)
{
    m_pwm.SetDutyCycle(dutyCycle);
}

void pwmConnection::SetCounts(unsigned int counts)
{
    m_pwm.SetCounts(counts);
}
