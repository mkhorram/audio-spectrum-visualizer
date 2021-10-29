#include "audioinputhandler.h"
#include <QDebug>

AudioInputHandler::AudioInputHandler()
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
    long long dataSize = IODevice->read(m_buf, m_buf_length);

    measureActualSampleRate(dataSize);

    qDebug() << dataSize;
}



