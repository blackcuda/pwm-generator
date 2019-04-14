#include "pwm.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include <QObject>


PWM::PWM(unsigned int cnts, pwmMode m)
    : m_counts(cnts)
    , m_mode(m)
    , m_clk(mapRegAddr(CLOCK_BASE))
    , m_gpio(mapRegAddr(GPIO_BASE))
    , m_pwm(mapRegAddr(PWM_BASE))
{
    std::cout << "Create PWM" << std::endl;
    configPwmPin1();
    //configPwm1();
}

PWM::~PWM()
{
    std::cout << "Destroy PWM" << std::endl;
    setPwmControlReg(0);
    setPwmRangeReg(0x20);
    setPwmDataReg(0);

    if (munmap((void*)m_pwm, BLOCK_SIZE) < 0)
    {
        perror("munmap (pwm) failed");
        exit(1);
    }

    killPwmClock();

    while ((*(m_clk + PWMCLK_CNTL)) & 0x00000080)
    {
        // Wait until busy flag is set
    }

    resetDivisor();
    enableClock();

    // Unmap the memory block containing the PWM Clock Registers
    if (munmap((void*)m_clk, BLOCK_SIZE) < 0)
    {
        perror("munmap (clk) failed");
        exit(1);
    }

    // Put the GPIO pheripheral registers in their original state
    // Set to input mode (default)
    *(m_gpio + 1) &= ~(7 << 24);

    if (munmap((void*)m_gpio, BLOCK_SIZE) < 0)
    {
        perror("munmap (gpio) failed");
        exit(1);
    }
}

void PWM::StartPwm()
{
    configPwm1();
}

void PWM::StopPwm()
{
    disablePwm();
    setPwmRangeReg(0x20);
    setPwmDataReg(0);
}

void PWM::SetFrequency(unsigned int frequency)
{
    std::cout << "Frequency PWM = " << frequency << std::endl;
    if (frequency > 1 && frequency < 19200000)
    {
//      double period = calculatePeriod(frequency);
//      double countDuration = calculateCountDuration(period, m_counts);
        unsigned int divisor = calculateDivisor(frequency, m_counts);

        killPwmClock();

        while ((*(m_clk + PWMCLK_CNTL)) & 0x00000080)
        {
            /* Wait until busy flag is set */
        }

        setDivisor(divisor);
        enableClock();
    }
}

void PWM::SetDutyCycle(unsigned int dutyCycle)
{
    std::cout << "Set Duty Cycle" << std::endl;
    unsigned int bitCount = 0;

    if ((dutyCycle < 1) || dutyCycle > 99)
    {
        std::cout << "Duty Cycle must be between 1 and 99" << std::endl;
        return;
    }

    bitCount = calculateBitCount(dutyCycle, m_counts);
    setPwmDataReg(bitCount);

    usleep(10);
}

void PWM::SetCounts(unsigned int counts)
{
    std::cout << "Set number of counts"  << std::endl;
    m_counts = counts;

    setPwmRangeReg(m_counts);

    usleep(10);
}

volatile unsigned *PWM::mapRegAddr(unsigned long baseAddr)
{ 
    int mem_fd =0;
    void *regAddrMap = MAP_FAILED;

    if (!mem_fd)
    {
        if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0)
        {
            perror("Can't open /dev/mem");
            exit(1);
        }
    }

    /* mmap IO */
    regAddrMap = mmap(NULL,
                      BLOCK_SIZE,
                      PROT_READ | PROT_WRITE | PROT_EXEC,
                      MAP_SHARED | MAP_LOCKED,
                      mem_fd,
                      baseAddr);

    if (regAddrMap == MAP_FAILED)
    {
        perror("mmap error");
        close(mem_fd);
        exit(1);
    }

    if (close(mem_fd) < 0)
    {
        perror("Couldn't close /dev/mem file descriptor");
        exit(1);
    }

    std::cout << "Map Register Address - Base address = " << baseAddr << "- Address Map = " << regAddrMap << std::endl;
    //std::cout << "Map Register Address - Base address = " << std::hex << baseAddr << " - regAddrMap = " << std::hex << regAddrMap << std::endl;

    return (volatile unsigned *)regAddrMap;
}

void PWM::configPwmPin1()
{
    std::cout << "Config PWM Pin 1" << std::endl;

    std::cout << "GPIO Address = " << m_gpio << std::endl;
    std::cout << "Set GPIO18 as input" << std::endl;
    /* Set GPIO18 as input */
    *(m_gpio + 1) &= ~(7 << 24);


    std::cout << "Set GPIO18 to ALT5 function PWM0"  << std::endl;
    /* Set GPIO18 to ALT5 function PWM0 */
    *(m_gpio + 1) |= (2 << 24);
}

void PWM::configPwm1()
{
    std::cout << "Config PWM 1"  << std::endl;
    const double frequency = 1000;
    double dutyCycle = 50;

    SetFrequency(frequency);
    disablePwm();

    // Set the number of counts that constitute a period with 0 for 20 milliseconds = 320 bits.
    SetCounts(m_counts);
    SetDutyCycle(dutyCycle);

    std::cout << "Start PWM1"  << std::endl;
    // Start PWM1
    if (m_mode == pwmMode::PWM_MODE)
    {
        *(m_pwm + PWM_CTL) |= (1 << 0);
    }
    else // M/S mode
    {
        *(m_pwm + PWM_CTL) |= ((1 << 7) | (1 << 0));
    }

    std::cout << "End Config PWM1" << std::endl;
}

void PWM::disablePwm()
{
    setPwmControlReg(0);

    usleep(10);
}

void PWM::killPwmClock()
{
    std::cout << "Kill PWM clock"  << std::endl;
    *(m_clk + PWMCLK_CNTL) = 0x5A000000 | (1 << 5);

    usleep(10);
}

double PWM::calculatePeriod(int frequency)
{
    return (1.0 / frequency);
}

double PWM::calculateCountDuration(double period, unsigned int counts)
{
    return (period / (counts * 1.0f));
}

unsigned int PWM::calculateDivisor(double countDuration)
{
    std::cout << "countduration = " << countDuration << std::endl;
    unsigned int divisor = (int)(192000000.0f / (1.0 / countDuration));

    std::cout << "Divisor = " << divisor << std::endl;

    return divisor;
}

unsigned int PWM::calculateDivisor(int frequency, unsigned int counts)
{
    unsigned int divisor = (int)(192000000.0f / frequency / counts);

    if (divisor <= 1 || divisor > 4095)
    {
        if (divisor <= 1)
        {
            std::cout << "divisor = 0, m_counts = " << m_counts << std::endl;
            SetCounts(m_counts >> 1);
            std::cout << "m_counts = " << m_counts << std::endl;
        }

        if (divisor > 4095)
        {
            SetCounts(m_counts << 1);
        }

        calculateDivisor(frequency, m_counts);
    }

    std::cout << "Divisor = " << divisor << std::endl;

    return divisor;
}

unsigned int PWM::calculateBitCount(double dutyCycle, unsigned int counts)
{
    return ((int) ((dutyCycle / 100) * counts));
}

void PWM::setDivisor(unsigned int divisor)
{
    if (divisor == 0 ||divisor > 4095)
    {
        printf("Divisor value must be between 0 - 4095 \n");
        exit(-1);
    }

    m_divisor = divisor;

    std::cout << "Set Divisor = " << m_divisor << std::endl;

    // Set divisor
    setPwmDivisorReg(0x5A000000 | (m_divisor << 12));
}

void PWM::resetDivisor()
{
    setPwmDivisorReg(0x5A000000);
    usleep(10);
}

void PWM::enableClock()
{
    std::cout << "Enable Clock"  << std::endl;
    // Source = osc and enable clock
    *(m_clk + PWMCLK_CNTL) = 0x5A000011;
}

void PWM::setPwmDivisorReg(unsigned int divisor)
{
    *(m_clk + PWMCLK_DIV) = divisor;
}

void PWM::setPwmControlReg(unsigned int control)
{
    *(m_pwm + PWM_CTL) = control;
}

void PWM::setPwmDataReg(unsigned int data)
{
    *(m_pwm + PWM_DAT1) = data;
}

void PWM::setPwmRangeReg(unsigned int range)
{
    *(m_pwm + PWM_RNG1) = range;
}
