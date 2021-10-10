#include "dialogsettings.h"
#include "ui_dialogsettings.h"

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);
    on_btnRefresh_clicked();
}

DialogSettings::~DialogSettings()
{
    delete ui;
}


void DialogSettings::closeEvent(QCloseEvent *event)
{
    emit DialogSettingsClosed(event);
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

void DialogSettings::on_btnCancel_clicked()
{
    this->close();
}


void DialogSettings::deviceChanged(int idx)
{
    if (ui->cboActiveAudioInput->count() == 0)
        return;

    QStringList fields;

    m_deviceInfo = ui->cboActiveAudioInput->itemData(idx).value<QAudioDeviceInfo>();

    ui->cboSampleRate->clear();
    fields.clear();
    QList<int> sampleRatez = m_deviceInfo.supportedSampleRates();
    for (int i = 0; i < sampleRatez.size(); ++i)
        fields << QString("%1").arg(sampleRatez.at(i));
    ui->cboSampleRate->addItems(fields);
    if (sampleRatez.size() > 0)
        m_settings.setSampleRate(sampleRatez.at(0));

    ui->cboChannel->clear();
    fields.clear();
    QList<int> chz = m_deviceInfo.supportedChannelCounts();
    for (int i = 0; i < chz.size(); ++i)
        fields << QString("%1").arg(chz.at(i));
    ui->cboChannel->addItems(fields);
    if (chz.size())
        m_settings.setChannelCount(chz.at(0));

    ui->cboCodecs->clear();
    fields.clear();
    QStringList codecs = m_deviceInfo.supportedCodecs();
    for (int i = 0; i < codecs.size(); ++i)
        fields << QString("%1").arg(codecs.at(i));
    ui->cboCodecs->addItems(fields);
    if (codecs.size())
        m_settings.setCodec(codecs.at(0));

    ui->cboBitDepth->clear();
    fields.clear();
    QList<int> sampleSizez = m_deviceInfo.supportedSampleSizes();
    for (int i = 0; i < sampleSizez.size(); ++i)
        fields << QString("%1").arg(sampleSizez.at(i));
    ui->cboBitDepth->addItems(fields);
    if (sampleSizez.size())
        m_settings.setSampleSize(sampleSizez.at(0));

    ui->cboSampleType->clear();
    fields.clear();
    QList<QAudioFormat::SampleType> sampleTypez = m_deviceInfo.supportedSampleTypes();

    for (int i = 0; i < sampleTypez.size(); ++i)
    {
        switch (sampleTypez[i]) {
        case QAudioFormat::SignedInt:
            fields << "SignedInt";
            break;
        case QAudioFormat::UnSignedInt:
            fields << "UnSignedInt";
            break;
        case QAudioFormat::Float:
            fields << "Float";
            break;
        case QAudioFormat::Unknown:
            fields << "Unknown";
        }
    }
    ui->cboSampleType->addItems(fields);
    if (sampleTypez.size() > 0)
        m_settings.setSampleType(sampleTypez.at(0));

    ui->cboEndianness->clear();
    fields.clear();
    QList<QAudioFormat::Endian> endianz = m_deviceInfo.supportedByteOrders();
    for (int i = 0; i < endianz.size(); ++i)
    {
        switch (endianz[i]) {
        case QAudioFormat::LittleEndian:
            fields << "LittleEndian";
            break;
        case QAudioFormat::BigEndian:
            fields << "BigEndian";
            break;
        default:
            fields << "Unknown";
        }
    }
    ui->cboEndianness->addItems(fields);
    if (endianz.size() > 0)
        m_settings.setByteOrder(endianz.at(0));
}
