#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QString>

MainWindow::MainWindow(pwmData& aPwmData, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pwmData(aPwmData)
{
    ui->setupUi(this);

    connectSignalsAndSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSignalsAndSlots(void)
{
    connect(ui->OnOffCheckBox, &QCheckBox::toggled, this, &MainWindow::OnOffCheckboxToggled);
    connect(ui->frequencySlider, &QSlider::valueChanged, this, &MainWindow::FrequencySliderValueChanged);
    connect(ui->dutyCycleSlider, &QSlider::valueChanged, this, &MainWindow::DutyCycleSliderValueChanged);

    connect(ui->frequencySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::FrequencySpinBoxValueChanged);
    connect(ui->dutyCycleSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::DutyCycleSpinBoxValueChanged);

    connect(ui->frequencyComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::FrequencyComboBoxIndexChanged);

    connect(&m_pwmData, &pwmData::dutyCycleChanged, this, &MainWindow::DutyCycleSliderValueChanged);
    connect(&m_pwmData, &pwmData::dutyCycleChanged, this, &MainWindow::DutyCycleSpinBoxValueChanged);
    connect(&m_pwmData, &pwmData::frequencyChanged, this, &MainWindow::FrequencySliderValueChanged);
    connect(&m_pwmData, &pwmData::frequencyChanged, this, &MainWindow::FrequencySpinBoxValueChanged);
}

void MainWindow::OnOffCheckboxToggled(bool checked)
{
    m_pwmData.SetPwmOn(checked);
}

void MainWindow::FrequencySliderValueChanged(double value)
{
    m_pwmData.SetFrequency(value); // convert to KHz later on.

    ui->frequencySlider->setValue(value);
}

void MainWindow::DutyCycleSliderValueChanged(double value)
{
    m_pwmData.SetDutyCycle(value);

    ui->dutyCycleSlider->setValue(value);
}

void MainWindow::FrequencySpinBoxValueChanged(double value)
{
    m_pwmData.SetFrequency(value);

    ui->frequencySpinBox->setValue(value);
}

void MainWindow::DutyCycleSpinBoxValueChanged(double value)
{
    m_pwmData.SetDutyCycle(value);

    ui->dutyCycleSpinBox->setValue(value);
}

void MainWindow::FrequencyComboBoxIndexChanged(unsigned int index)
{
    std::cout << "Frequency Combobox =  " << index << std::endl;

    switch(index)
    {
    case 0:
        m_pwmData.SetFrequencyUnit(index);
        break;
    case 1:
        break;
    default:
        break;
    }

}
