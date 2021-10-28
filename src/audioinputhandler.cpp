#include "audioinputhandler.h"
#include <QDebug>

AudioInputHandler::AudioInputHandler()
{
    m_buf = new char[m_buf_length];
}

AudioInputHandler::~AudioInputHandler()
{
    delete m_buf;
}



bool AudioInputHandler::start(QAudioFormat format, QAudioDeviceInfo audioDeviceInfo)
{
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

    tp = std::chrono::high_resolution_clock::now();
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
    std::chrono::duration<int, std::milli> d(10);
    std::chrono::time_point<std::chrono::high_resolution_clock> tp2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dt = tp2-tp;
    tp = tp2;

    QByteArray data = IODevice->readAll();
    data.size();

    qDebug() << data.size() << "   time duration (millisec: " << "   " << static_cast<int> (dt.count()*1000);

}



