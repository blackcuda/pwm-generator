#include "pwmcontroller.h"

pwmController::pwmController(pwmData& aPwmData, pwmConnection& aPwmConnection)
    : m_pwmData(aPwmData)
    , m_pwmConnection(aPwmConnection)
{
    connectSignalsAndSlots();
}

void pwmController::connectSignalsAndSlots()
{
    connect(&m_pwmData, &pwmData::pwmOnChanged, this, &pwmController::SetPwmOn);

    connect(&m_pwmData, &pwmData::frequencyChanged, this, &pwmController::UpdateFrequency);
    connect(&m_pwmData, &pwmData::dutyCycleChanged, this, &pwmController::UpdateDutyCycle);
}

void pwmController::SetPwmOn(bool on)
{
    if(on)
    {
        m_pwmConnection.SetPwmOn();
    }
    else
    {
        m_pwmConnection.SetPwmOff();
    }
}

void pwmController::UpdateFrequency(double frequency)
{
    if (!m_pwmData.IsPwmOn())
    {
        return;
    }

    double convertedFrequency = convertFrequency(frequency);

    m_pwmConnection.SetFrequency(convertedFrequency);
}

void pwmController::UpdateDutyCycle(double dutyCycle)
{
    if (!m_pwmData.IsPwmOn())
    {
        return;
    }

    m_pwmConnection.SetDutyCycle(dutyCycle);
}

double pwmController::convertFrequency(double frequency)
{
    m_pwmData.GetFrequencyUnit();

    double convertedFrequency = frequency * 1000;

    return convertedFrequency;
}
