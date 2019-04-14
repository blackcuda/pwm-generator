#include "pwmdata.h"

#include <iostream>

pwmData::pwmData()
    : m_pwmOn(false)
    , m_frequency(1000)
    , m_dutyCycle(50)
    , m_frequencyUnit(0)
{

}

void pwmData::SetPwmOn(bool on)
{
    if (m_pwmOn == on)
    {
        return;
    }

    std::cout << "SET PWM " << on << std::endl;

    m_pwmOn = on;

    emit pwmOnChanged(m_pwmOn);
}

void pwmData::SetFrequency(double frequency)
{
    if (m_frequency == frequency)
    {
        return;
    }

    m_frequency = frequency;

    std::cout << "m_frequency = " << m_frequency << std::endl;

    emit frequencyChanged(m_frequency);
}

void pwmData::SetDutyCycle(double dutyCycle)
{
    if (m_dutyCycle == dutyCycle)
    {
        return;
    }

    m_dutyCycle = dutyCycle;

    emit dutyCycleChanged(m_dutyCycle);
}

void pwmData::SetCounts(unsigned int counts)
{
    if (m_counts == counts)
    {
        return;
    }

    m_counts = counts;

    emit countsChanged(m_counts);
}

void pwmData::SetFrequencyUnit(unsigned int unit)
{
    if(m_frequencyUnit == unit)
    {
        return;
    }

    m_frequencyUnit = unit;

    emit frequencyUnitChanged(unit);
}

double pwmData::GetFrequency(void)
{
    return m_frequency;
}

double pwmData::GetDutyCycle(void)
{
    return m_dutyCycle;
}

unsigned int pwmData::GetCounts(void)
{
    return m_counts;
}

bool pwmData::IsPwmOn(void)
{
    return m_pwmOn;
}

int pwmData::GetFrequencyUnit(void)
{
    return m_frequencyUnit;
}
