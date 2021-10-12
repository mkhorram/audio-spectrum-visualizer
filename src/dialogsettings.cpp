#include "dialogsettings.h"
#include "ui_dialogsettings.h"

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);
    this->setModal(true);

    on_btnRefresh_clicked();
    setGlobalFormatSettings();
}

DialogSettings::~DialogSettings()
{
    delete ui;
}


void DialogSettings::closeEvent(QCloseEvent *event)
{
    emit DialogSettingsClosed(event);
}

void DialogSettings::showEvent(QShowEvent *event)
{
//TODO: restore the default settings
    QString searchString;

//sampleRate() = -1
    searchString = QString::number(m_globalFormatSettings.sampleRate());
    for (int ii = 0; ii<ui->cboSampleRate->count(); ++ii)
        if (ui->cboSampleRate->itemText(ii) == searchString)
        {
            ui->cboSampleRate->setCurrentIndex(ii);
            break;
        }
    //
    // //channelCount() = -1
    //     if (ui->cboChannel->count())
    //         m_globalFormatSettings.setChannelCount(1);
    //
//sampleSize() = -1
    searchString = QString::number(m_globalFormatSettings.sampleSize());
    for (int ii = 0; ii<ui->cboSampleSize->count(); ++ii)
        if (ui->cboSampleSize->itemText(ii) == searchString)
        {
            ui->cboSampleSize->setCurrentIndex(ii);
            break;
        }

//byteOrder() = QAudioFormat::Endian(QSysInfo::ByteOrder)
    searchString = (m_globalFormatSettings.byteOrder() == QAudioFormat::LittleEndian)? "LittleEndian" : "BigEndian";
    for (int ii = 0; ii<ui->cboEndianness->count(); ++ii)
        if (ui->cboEndianness->itemText(ii) == searchString)
        {
            ui->cboEndianness->setCurrentIndex(ii);
            break;
        }

    // //sampleType() = QAudioFormat::Unknown codec() = ""
    //     if (ui->cboSampleType->count())
    //     {
    //         if (ui->cboSampleType->currentText() == "SignedInt")
    //             m_globalFormatSettings.setSampleType(QAudioFormat::SignedInt);
    //         else if (ui->cboSampleType->currentText() == "UnSignedInt")
    //             m_globalFormatSettings.setSampleType(QAudioFormat::UnSignedInt);
    //         else if (ui->cboSampleType->currentText() == "Float")
    //             m_globalFormatSettings.setSampleType(QAudioFormat::Float);
    //         else
    //             m_globalFormatSettings.setSampleType(QAudioFormat::Unknown);
    //     }
    //
    //     if (ui->cboCodecs->count())
    //         m_globalFormatSettings.setCodec(ui->cboCodecs->currentText());
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

void DialogSettings::on_btnOK_clicked()
{
    setGlobalFormatSettings();
    close();
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
        m_globalFormatSettings.setSampleRate(sampleRatez.at(0));

    ui->cboChannel->clear();
    fields.clear();
    QList<int> chz = m_deviceInfo.supportedChannelCounts();
    for (int i = 0; i < chz.size(); ++i)
        fields << QString("%1").arg(chz.at(i));
    ui->cboChannel->addItems(fields);
    if (chz.size())
        m_globalFormatSettings.setChannelCount(chz.at(0));

    ui->cboCodecs->clear();
    fields.clear();
    QStringList codecs = m_deviceInfo.supportedCodecs();
    for (int i = 0; i < codecs.size(); ++i)
        fields << QString("%1").arg(codecs.at(i));
    ui->cboCodecs->addItems(fields);
    if (codecs.size())
        m_globalFormatSettings.setCodec(codecs.at(0));

    ui->cboSampleSize->clear();
    fields.clear();
    QList<int> sampleSizez = m_deviceInfo.supportedSampleSizes();
    for (int i = 0; i < sampleSizez.size(); ++i)
        fields << QString("%1").arg(sampleSizez.at(i));
    ui->cboSampleSize->addItems(fields);
    if (sampleSizez.size())
        m_globalFormatSettings.setSampleSize(sampleSizez.at(0));

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
        m_globalFormatSettings.setSampleType(sampleTypez.at(0));

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
        m_globalFormatSettings.setByteOrder(endianz.at(0));
}

void DialogSettings::setGlobalFormatSettings()
{
//sampleRate() = -1
    if (ui->cboSampleRate->count())
        m_globalFormatSettings.setSampleRate(ui->cboSampleRate->currentText().toInt());

//channelCount() = -1
    if (ui->cboChannel->count())
        m_globalFormatSettings.setChannelCount(1);

//sampleSize() = -1
    if (ui->cboSampleSize->count())
    {
        int i = ui->cboSampleSize->currentText().toInt();
        m_globalFormatSettings.setSampleSize(ui->cboSampleSize->currentText().toInt());
    }

//byteOrder() = QAudioFormat::Endian(QSysInfo::ByteOrder)
    if (ui->cboEndianness->count())
        m_globalFormatSettings.setByteOrder((ui->cboEndianness->currentText() == "LittleEndian")?
                                                 QAudioFormat::LittleEndian : QAudioFormat::BigEndian);

//sampleType() = QAudioFormat::Unknown codec() = ""
    if (ui->cboSampleType->count())
    {
        if (ui->cboSampleType->currentText() == "SignedInt")
            m_globalFormatSettings.setSampleType(QAudioFormat::SignedInt);
        else if (ui->cboSampleType->currentText() == "UnSignedInt")
            m_globalFormatSettings.setSampleType(QAudioFormat::UnSignedInt);
        else if (ui->cboSampleType->currentText() == "Float")
            m_globalFormatSettings.setSampleType(QAudioFormat::Float);
        else
            m_globalFormatSettings.setSampleType(QAudioFormat::Unknown);
    }

    if (ui->cboCodecs->count())
        m_globalFormatSettings.setCodec(ui->cboCodecs->currentText());
}
