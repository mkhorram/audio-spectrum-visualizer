#include "audioinputhandler.h"
#include <QDebug>

AudioInputHandler::AudioInputHandler(long long buf_length) : m_buf_length(buf_length)
{
    m_buf = new char[m_buf_length];
    m_samples.resize(m_buf_length);
}

AudioInputHandler::~AudioInputHandler()
{
    delete m_buf;
}



bool AudioInputHandler::start(QAudioFormat format, QAudioDeviceInfo audioDeviceInfo)
{
    Q_ASSERT(format.sampleSize() % 8 == 0);
    m_minSampleCount = format.sampleRate();

    m_format = format;
    m_audioDeviceInfo = audioDeviceInfo;
    if (m_audioDeviceInfo.isNull() || !m_format.isValid())
        return false;

    if (m_AudioInput != nullptr) delete m_AudioInput;
    m_AudioInput = nullptr;

    m_AudioInput = new QAudioInput(m_audioDeviceInfo, m_format, this);
    m_notifyInterval = m_AudioInput->notifyInterval();

    QObject::connect(m_AudioInput, &QAudioInput::notify, this, &AudioInputHandler::processAudioIn);
    QObject::connect(m_AudioInput, &QAudioInput::stateChanged, this, &AudioInputHandler::stateChangeAudioIn);

    IODevice = m_AudioInput->start();
    QObject::connect(IODevice, &QBuffer::readyRead, this, &AudioInputHandler::readyRead);

    m_timePoint = std::chrono::high_resolution_clock::now();
    qDebug() << "Device " << audioDeviceInfo.deviceName();
    qDebug() << "sampleRate " << format.sampleRate();
    qDebug() << "sampleSize " << format.sampleSize();

    return true;
}

void AudioInputHandler::stop()
{
    if (m_AudioInput == nullptr)
        return;
    m_AudioInput->stop();
    m_AudioInput->deleteLater();
    m_AudioInput = nullptr;
}

void AudioInputHandler::measureActualSampleRate(long long newReceivedSampleCount)
{
    m_sampleCount += newReceivedSampleCount;
    if (m_sampleCount >= m_minSampleCount)
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> timePoint2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double>  dt = timePoint2-m_timePoint;
        long actualSampleRate = static_cast<long>(m_sampleCount / dt.count());
        qDebug() << "Sample Rate: " << actualSampleRate << " = " << m_sampleCount << " / " << dt.count();
        m_timePoint = timePoint2;
        m_sampleCount = 0;
    }
}

void AudioInputHandler::processAudioIn()
{
    //
}

void AudioInputHandler::stateChangeAudioIn(QAudio::State s)
{
    //
}

void AudioInputHandler::readyRead()
{
    long dataSize = IODevice->read(m_buf, m_buf_length);

    measureActualSampleRate(dataSize);

    if (m_format.sampleSize() == 8)
    {
        if (m_format.sampleType() == QAudioFormat::UnSignedInt)
            castDataToDouble<quint8>(m_buf, dataSize);
        else if (m_format.sampleType() == QAudioFormat::SignedInt)
            castDataToDouble<qint8>(m_buf, dataSize);
    }
    else if (m_format.sampleSize() == 16)
    {
        if (m_format.sampleType() == QAudioFormat::UnSignedInt)
            castDataToDouble<quint16>(m_buf, dataSize);
        else if (m_format.sampleType() == QAudioFormat::SignedInt)
            castDataToDouble<qint16>(m_buf, dataSize);
    }
    else if (m_format.sampleSize() == 32)
    {
        if (m_format.sampleType() == QAudioFormat::UnSignedInt)
            castDataToDouble<quint32>(m_buf, dataSize);
        else if (m_format.sampleType() == QAudioFormat::SignedInt)
            castDataToDouble<qint32>(m_buf, dataSize);
    }
    else if (m_format.sampleSize() == 64)
    {
        if (m_format.sampleType() == QAudioFormat::UnSignedInt)
            castDataToDouble<quint64>(m_buf, dataSize);
        else if (m_format.sampleType() == QAudioFormat::SignedInt)
            castDataToDouble<qint64>(m_buf, dataSize);
    }
    else if (m_format.sampleType() == QAudioFormat::Float)
    {
        castDataToDouble<float>(m_buf, dataSize);
    }

    qDebug() << dataSize;
}



