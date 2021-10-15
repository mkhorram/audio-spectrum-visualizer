#include "dialogsettings.h"
#include "ui_dialogsettings.h"

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);
    ui->cboChannel->setVisible(false);
    ui->lblChannel->setVisible(false);
    this->setModal(true);

    on_btnRefresh_clicked();
    setGlobalFormatSettings();

    // disabled until implementation
    ui->tabFFT->setEnabled(false);
    ui->tabVisualization->setEnabled(false);
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

QAudioFormat DialogSettings::getFormat()
{
    return m_globalFormatSettings;
}

QAudioDeviceInfo DialogSettings::getDeviceInfo()
{
    return m_deviceInfo;
}


void DialogSettings::closeEvent(QCloseEvent *event)
{
    emit DialogSettingsClosed(event);
}

void DialogSettings::showEvent(QShowEvent *event)
{
//TODO: restore the default settings
    ui->cboActiveAudioInput->setCurrentIndex(m_deviceIndex);

    QString searchString;

//sampleRate() = -1
    searchString = QString::number(m_globalFormatSettings.sampleRate());
    for (int ii = 0; ii < ui->cboSampleRate->count(); ++ii)
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
    for (int ii = 0; ii < ui->cboSampleSize->count(); ++ii)
        if (ui->cboSampleSize->itemText(ii) == searchString)
        {
            ui->cboSampleSize->setCurrentIndex(ii);
            break;
        }

//byteOrder() = QAudioFormat::Endian(QSysInfo::ByteOrder)
    searchString = (m_globalFormatSettings.byteOrder() == QAudioFormat::LittleEndian)? "LittleEndian" : "BigEndian";
    for (int ii = 0; ii < ui->cboEndianness->count(); ++ii)
        if (ui->cboEndianness->itemData(ii).value<QAudioFormat::Endian>() == m_globalFormatSettings.byteOrder())
        {
            ui->cboEndianness->setCurrentIndex(ii);
            break;
        }

//sampleType() = QAudioFormat::Unknown codec() = ""
    for (int ii = 0; ii < ui->cboSampleType->count(); ++ii)
        if (ui->cboSampleType->itemData(ii).value<QAudioFormat::SampleType>() == m_globalFormatSettings.sampleType())
        {
            ui->cboSampleType->setCurrentIndex(ii);
            break;
        }

    searchString = m_globalFormatSettings.codec();
    for (int ii = 0; ii < ui->cboCodecs->count(); ++ii)
        if (ui->cboCodecs->itemText(ii) == searchString)
        {
            ui->cboCodecs->setCurrentIndex(ii);
            break;
        }
}

void DialogSettings::on_btnRefresh_clicked()
{
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    ui->cboActiveAudioInput->clear();
    for (auto &deviceInfo: QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
        ui->cboActiveAudioInput->addItem(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));

    m_deviceIndex = 0;
    ui->cboActiveAudioInput->setCurrentIndex(m_deviceIndex);
    deviceChanged(m_deviceIndex);
}

void DialogSettings::on_btnCancel_clicked()
{
    this->close();
}

void DialogSettings::on_btnOK_clicked()
{
    m_deviceIndex = ui->cboActiveAudioInput->currentIndex();
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
    QList<QAudioFormat::SampleType> sampleTypez = m_deviceInfo.supportedSampleTypes();

    for (int i = 0; i < sampleTypez.size(); ++i)
    {
        switch (sampleTypez[i]) {
        case QAudioFormat::SignedInt:
            ui->cboSampleType->addItem("SignedInt", qVariantFromValue(QAudioFormat::SignedInt));
            break;
        case QAudioFormat::UnSignedInt:
            ui->cboSampleType->addItem("UnSignedInt", qVariantFromValue(QAudioFormat::UnSignedInt));
            break;
        case QAudioFormat::Float:
            ui->cboSampleType->addItem("Float", qVariantFromValue(QAudioFormat::Float));
            break;
        case QAudioFormat::Unknown:
            ui->cboSampleType->addItem("Unknown", qVariantFromValue(QAudioFormat::Unknown));
        }
    }
    if (sampleTypez.size() > 0)
        m_globalFormatSettings.setSampleType(sampleTypez.at(0));

    ui->cboEndianness->clear();
    QList<QAudioFormat::Endian> endianz = m_deviceInfo.supportedByteOrders();
    for (int i = 0; i < endianz.size(); ++i)
    {
        switch (endianz[i]) {
        case QAudioFormat::LittleEndian:
            ui->cboEndianness->addItem("LittleEndian", qVariantFromValue(QAudioFormat::LittleEndian));
            break;
        case QAudioFormat::BigEndian:
            ui->cboEndianness->addItem("BigEndian", qVariantFromValue(QAudioFormat::BigEndian));
            break;
        }
    }
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
        m_globalFormatSettings.setByteOrder(
                    ui->cboEndianness->itemData(ui->cboEndianness->currentIndex()).value<QAudioFormat::Endian>());

//sampleType() = QAudioFormat::Unknown codec() = ""
    if (ui->cboSampleType->count())
    {
        m_globalFormatSettings.setSampleType(
                    ui->cboSampleType->itemData(ui->cboSampleType->currentIndex()).value<QAudioFormat::SampleType>());
    }

    if (ui->cboCodecs->count())
        m_globalFormatSettings.setCodec(ui->cboCodecs->currentText());
}
