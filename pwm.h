#ifndef PWM_H
#define PWM_H

#include "pwmdata.h"

enum pwmMode
{
  PWM_MODE = 1,
  MS_MODE = 2
};

class PWM
{
public:
    explicit PWM(unsigned int cnts, pwmMode m);

    ~PWM();

    void StartPwm();
    void StopPwm();

    void SetFrequency(unsigned int frequency);
    void SetDutyCycle(unsigned int dutyCycle);
    void SetCounts(unsigned int m_counts);

private:

    const int BCM2708_PERI_BASE = 0x20000000;
    const int PWM_BASE = (BCM2708_PERI_BASE + 0x20C000); /* PWM controller */
    const int CLOCK_BASE = (BCM2708_PERI_BASE + 0x101000); /* Clock controller */
    const int GPIO_BASE = (BCM2708_PERI_BASE + 0x200000); /* GPIO controller */

    const int PWM_CTL = 0;
    const int PWM_RNG1 = 4;
    const int PWM_DAT1 = 5;
    const int PWMCLK_CNTL = 40;
    const int PWMCLK_DIV = 41;

    const int BLOCK_SIZE = 4096;

    volatile unsigned *mapRegAddr(unsigned long baseAddr);

    void configPwmPin1();
    void configPwm1();

    void disablePwm();

    void killPwmClock();
    double calculatePeriod(int frequency);
    double calculateCountDuration(double period, unsigned int counts);
    unsigned int calculateDivisor(double countDuration);
    unsigned int calculateDivisor(int frequency, unsigned int counts);
    unsigned int calculateBitCount(double dutyCycle, unsigned int counts);

    void setDivisor(unsigned int divisor);
    void resetDivisor();
    void enableClock();

    void setPwmDivisorReg(unsigned int divisor);
    void setPwmControlReg(unsigned int control);
    void setPwmDataReg(unsigned int data);
    void setPwmRangeReg(unsigned int range);

    unsigned int m_counts;
    unsigned int m_divisor;
    int m_mode;

    volatile unsigned *m_clk;
    volatile unsigned *m_gpio;
    volatile unsigned *m_pwm;

};

#endif // PWM_H
