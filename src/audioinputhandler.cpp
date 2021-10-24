#include "audioinputhandler.h"
#include <QDebug>

AudioInputHandler::AudioInputHandler()
{

}

AudioInputHandler::~AudioInputHandler()
{ }



bool AudioInputHandler::start(QAudioFormat format, QAudioDeviceInfo audioDeviceInfo)
{
    m_format = format;
    m_audioDeviceInfo = audioDeviceInfo;
    if (m_audioDeviceInfo.isNull() || !m_format.isValid())
        return false;

    if (m_AudioInput) delete m_AudioInput;
    m_AudioInput = nullptr;

    m_AudioInput = new QAudioInput(m_audioDeviceInfo, m_format, this);
    m_notifyInterval = m_AudioInput->notifyInterval();

    QObject::connect(m_AudioInput, &QAudioInput::notify, this, &AudioInputHandler::processAudioIn);
    QObject::connect(m_AudioInput, &QAudioInput::stateChanged, this, &AudioInputHandler::stateChangeAudioIn);
    QObject::connect(&m_InputBuffer, &QBuffer::readyRead, this, &AudioInputHandler::readyRead);

    m_InputBuffer.open(QBuffer::ReadWrite);
    m_AudioInput->start(&m_InputBuffer);

    return true;
}

void AudioInputHandler::stop()
{
    if (m_AudioInput == nullptr)
        return;
    m_AudioInput->stop();
    m_AudioInput->deleteLater();
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
    QByteArray data = m_InputBuffer.readAll();
    qDebug() << data.length();
}



