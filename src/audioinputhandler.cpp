#include "audioinputhandler.h"

AudioInputHandler::AudioInputHandler()
{

}

AudioInputHandler::~AudioInputHandler()
{
    //
}

void AudioInputHandler::setAudioFormat(QAudioFormat format)
{
    m_format = format;
}

void AudioInputHandler::setAudioDeviceInfo(QAudioDeviceInfo audioDeviceInfo)
{
    m_audioDeviceInfo = audioDeviceInfo;
}

bool AudioInputHandler::run()
{
    if (m_audioDeviceInfo.isNull())
        return false;

    if (m_AudioInput) delete m_AudioInput;

    m_AudioInput = nullptr;
    m_AudioInput = new QAudioInput(m_audioDeviceInfo, m_format, this);
    m_notifyInterval = m_AudioInput->notifyInterval();

    QObject::connect(m_AudioInput, &QAudioInput::notify, this, &AudioInputHandler::processAudioIn);
    QObject::connect(m_AudioInput, &QAudioInput::stateChanged, this, &AudioInputHandler::stateChangeAudioIn);

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



