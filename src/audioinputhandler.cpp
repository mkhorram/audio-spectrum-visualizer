#include "audioinputhandler.h"

#include <dj_fft.h>

AudioInputHandler::AudioInputHandler(unsigned long long buf_length) : m_buf_length(buf_length)
{
    m_buf = new char[m_buf_length];
    m_FFTSamples.reserve(m_buf_length);
}

AudioInputHandler::~AudioInputHandler()
{
    delete m_buf;
}



bool AudioInputHandler::start(QAudioFormat format, QAudioDeviceInfo audioDeviceInfo,
                              unsigned long long FFTNeededSamples /* must be 2^n */,
                              unsigned long long frequencyNeededSamples)
{
    Q_ASSERT(format.sampleSize() % 8 == 0);

    m_frequencyConversionRatio = double(m_format.sampleRate()) / FFTNeededSamples;

    m_FFTNeededSamples = FFTNeededSamples;
    m_FFTSamples.resize(m_FFTNeededSamples);

    m_frequencyNeededSamples = frequencyNeededSamples;
    m_frequencySampleCount = 0;

    m_format = format;
    m_audioDeviceInfo = audioDeviceInfo;
    if (m_audioDeviceInfo.isNull() || !m_format.isValid())
        return false;

    if (m_AudioInput != nullptr) delete m_AudioInput;
    m_AudioInput = new QAudioInput(m_audioDeviceInfo, m_format, this);
    m_notifyInterval = m_AudioInput->notifyInterval();

    QObject::connect(m_AudioInput, &QAudioInput::notify, this, &AudioInputHandler::processAudioIn);
    QObject::connect(m_AudioInput, &QAudioInput::stateChanged, this, &AudioInputHandler::stateChangeAudioIn);

    IODevice = m_AudioInput->start();
    QObject::connect(IODevice, &QBuffer::readyRead, this, &AudioInputHandler::readyRead);
    m_beginTimePoint = std::chrono::high_resolution_clock::now();

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

void AudioInputHandler::checkActualSampleRate(unsigned long long numSamples)
{
    m_frequencySampleCount += numSamples;
    if (m_frequencySampleCount >= m_frequencyNeededSamples)
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> endTimePoint = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double>  dt = endTimePoint - m_beginTimePoint;
        long actualSampleRate = static_cast<long>(m_frequencySampleCount / dt.count());
        m_beginTimePoint = endTimePoint;
        m_frequencySampleCount = 0;
        emit actualSampleRateEstimated(actualSampleRate);
    }
}

inline void AudioInputHandler::computeFFT()
{
    m_FFTOutput.rowBuffer = std::shared_ptr<std::vector<std::complex<double>>> (new std::vector<std::complex<double>>);
    *m_FFTOutput.rowBuffer = dj::fft1d(m_FFTSamples, dj::fft_dir::DIR_FWD);
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

    long sampleCount = 0;

    if (m_format.sampleSize() == 8)
    {
        if (m_format.sampleType() == QAudioFormat::UnSignedInt)
            sampleCount = castDataToDouble<quint8>(m_buf, dataSize);
        else if (m_format.sampleType() == QAudioFormat::SignedInt)
            sampleCount = castDataToDouble<qint8>(m_buf, dataSize);
    }
    else if (m_format.sampleSize() == 16)
    {
        if (m_format.sampleType() == QAudioFormat::UnSignedInt)
            sampleCount = castDataToDouble<quint16>(m_buf, dataSize);
        else if (m_format.sampleType() == QAudioFormat::SignedInt)
            sampleCount = castDataToDouble<qint16>(m_buf, dataSize);
    }
    else if (m_format.sampleSize() == 32)
    {
        if (m_format.sampleType() == QAudioFormat::UnSignedInt)
            sampleCount = castDataToDouble<quint32>(m_buf, dataSize);
        else if (m_format.sampleType() == QAudioFormat::SignedInt)
            sampleCount = castDataToDouble<qint32>(m_buf, dataSize);
    }
    else if (m_format.sampleSize() == 64)
    {
        if (m_format.sampleType() == QAudioFormat::UnSignedInt)
            sampleCount = castDataToDouble<quint64>(m_buf, dataSize);
        else if (m_format.sampleType() == QAudioFormat::SignedInt)
            sampleCount = castDataToDouble<qint64>(m_buf, dataSize);
    }
    else if (m_format.sampleType() == QAudioFormat::Float)
    {
        sampleCount = castDataToDouble<float>(m_buf, dataSize);
    }
    checkActualSampleRate(static_cast<unsigned long long>(sampleCount));
}



