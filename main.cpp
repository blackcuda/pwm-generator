#include "mainwindow.h"
#include <QApplication>

#include "pwmdata.h"
#include "pwmcontroller.h"
#include "pwm.h"
#include "pwmconnection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    pwmData aPwmData;
    PWM aPwm(256, pwmMode::MS_MODE);

    pwmConnection aPwmConnection(aPwm, aPwmData);

    pwmController aPwmController(aPwmData, aPwmConnection);

    MainWindow w(aPwmData);
    w.show();

    return a.exec();
}
