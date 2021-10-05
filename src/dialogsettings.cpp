#include "dialogsettings.h"
#include "ui_dialogsettings.h"

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::on_btnRefresh_clicked()
{
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    ui->cboActiveAudioInput->clear();
    for (auto &deviceInfo: QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
        ui->cboActiveAudioInput->addItem(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));

    ui->cboActiveAudioInput->setCurrentIndex(0);
    deviceChanged(0);
}

void DialogSettings::deviceChanged(int idx)
{}
