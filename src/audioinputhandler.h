#ifndef AUDIOINPUTHANDLER_H
#define AUDIOINPUTHANDLER_H

#include <QAudioInput>
#include <QBuffer>
#include <QVector>
#include <QAudioFormat>
#include <QIODevice>
#include <QObject>
#include <QtEndian>

#include <chrono>



#include "ringbuffer.hpp"


class AudioInputHandler : public QObject
{
    Q_OBJECT
private:
    QAudioInput *m_AudioInput = nullptr;
    QIODevice * IODevice;
    QAudioFormat m_format;
    QAudioDeviceInfo m_audioDeviceInfo;
    int m_channel = 0;

    long m_FFTNeededSamples;
    long m_FFTSampleCount;
    long m_frequencyNeededSamples;
    long m_frequencySampleCount;

    const long long m_buf_length;
    char * m_buf;
    RingBuffer<double> m_samples;
    int m_notifyInterval;

    std::chrono::time_point<std::chrono::high_resolution_clock> m_timePoint;

signals:

private slots:
    void processAudioIn();
    void stateChangeAudioIn(QAudio::State s);
    void readyRead();

public:
    explicit AudioInputHandler(long long buf_length = 100000);
    ~AudioInputHandler();

    bool start(QAudioFormat format, QAudioDeviceInfo audioDeviceInfo, long FFTNeededSamples, long frequencyNeededSamples);
    void stop();

private:
    void checkSampleCount(long long receivedBytesCount);

    template <typename T>
    long castDataToDouble(char * data, long len)
    {
        const int channelBytes = m_format.sampleSize() / 8;
        const int sampleBytes = m_format.channelCount() * channelBytes;
        Q_ASSERT(len % sampleBytes == 0);
        const long numSamples = len / sampleBytes;

        double maxVal = 0;
        double minVal = 0;

        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);
        ptr += (m_channel * channelBytes);

        if (m_format.byteOrder() == QAudioFormat::LittleEndian)
            for (int i = 0; i < numSamples; ++i)
            {
                double val =  qFromLittleEndian<T>(ptr);
                if (maxVal < val)
                    maxVal = val;
                else if (minVal > val)
                    minVal = val;
                m_samples.insert(val);
                ptr += sampleBytes;
            }
        else if (m_format.byteOrder() == QAudioFormat::BigEndian)
            for (int i = 0; i < numSamples; ++i)
            {
                double val =  qFromBigEndian<T>(ptr);
                if (maxVal < val)
                    maxVal = val;
                else if (minVal > val)
                    minVal = val;
                m_samples.insert(val);
                ptr += sampleBytes;
            }
        return numSamples;
    }
};

#endif // AUDIOINPUTHANDLER_H
