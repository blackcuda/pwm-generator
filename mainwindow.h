#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "pwmdata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(pwmData& aPwmData, QWidget *parent = 0);
    ~MainWindow();

public slots:
    void OnOffCheckboxToggled(bool checked);
    void FrequencySliderValueChanged(double value);
    void DutyCycleSliderValueChanged(double value);
    void FrequencySpinBoxValueChanged(double value);
    void DutyCycleSpinBoxValueChanged(double value);
    void FrequencyComboBoxIndexChanged(unsigned int index);

private:
    Ui::MainWindow *ui;
    pwmData& m_pwmData;

    void connectSignalsAndSlots(void);


};

#endif // MAINWINDOW_H
